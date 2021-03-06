// SPDX-License-Identifier: BSD-3-Clause
#include <cerrno>
#if defined(_MSC_VER) && !defined(_WINDOWS)
#	define _WINDOWS 1
#endif
#ifndef _WINDOWS
#	include <unistd.h>
#else
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <stringapiset.h>
#	include <fcntl.h>
#	include <io.h>
#	undef WIN32_LEAN_AND_MEAN
#	include <substrate/utility>
#endif
#include <substrate/console>

using substrate::operator ""_s;
using charTraits = std::char_traits<char>;

static const std::string errorPrefix{"[ERR]"_s};
static const std::string warningPrefix{"[WRN]"_s};
static const std::string infoPrefix{"[INF]"_s};
static const std::string debugPrefix{"[DBG]"_s};

#ifndef _WINDOWS
static const std::string colourRed{"\033[1;31m"_s};
static const std::string colourYellow{"\033[1;33m"_s};
static const std::string colourCyan{"\033[36m"_s};
static const std::string colourLightBlue{"\033[1;34m"_s};
static const std::string colourDefaults{"\033[0m"_s};
#endif

static const std::string nullString{"(null)"_s};
static const std::string trueString{"true"_s};
static const std::string falseString{"false"_s};

namespace substrate
{
	console_t console;

	void consoleStream_t::checkTTY() noexcept { _tty = isatty(fd); }

	void consoleStream_t::write(const void *const buffer, const size_t bufferLen) const noexcept
	{
		// We don't actually care if this succeeds. We just try if at all possible.
#ifndef _WINDOWS
		SUBSTRATE_NOWARN_UNUSED(const auto result) = ::write(fd, buffer, bufferLen);
#else
		SUBSTRATE_NOWARN_UNUSED(const auto result) = ::write(fd, buffer, uint32_t(bufferLen));
#endif
		errno = 0; // extra insurance.
	}

	// WARNING: This assumes you're giving it a TEXT stream so no non-printable stuff you want to preserve.
	// It will (if necessary) automatically UTF-8 => 16 convert whatever passes through for the sake of windows
	void consoleStream_t::write(const char *const value) const noexcept
	{
		if (value)
		{
#ifdef _WINDOWS
			const auto consoleMode = setmode(fd, _O_U8TEXT);
			const size_t valueLen = charTraits::length(value);
			const size_t stringLen = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED | MB_USEGLYPHCHARS,
				value, int(valueLen), nullptr, 0);
			auto string{make_unique_nothrow<wchar_t []>(stringLen)};
			if (!string)
				return;
			MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED | MB_USEGLYPHCHARS, value, int(valueLen),
				string.get(), int(stringLen));
			write(string.get(), sizeof(wchar_t) * stringLen);
			setmode(fd, consoleMode);
#else
			write(value, charTraits::length(value));
#endif
		}
		else
			write(nullString);
	}

	void consoleStream_t::write(const bool value) const noexcept
		{ write(value ? trueString : falseString); }

	console_t::console_t(FILE *const outStream, FILE *const errStream) noexcept :
		outputStream{fileno(outStream)}, errorStream{fileno(errStream)},
		valid_{outputStream.valid() && errorStream.valid()} { }

#ifndef _WINDOWS
	inline void red(const consoleStream_t &stream) noexcept
		{ stream.write(colourRed); }
	inline void yellow(const consoleStream_t &stream) noexcept
		{ stream.write(colourYellow); }
	inline void cyan(const consoleStream_t &stream) noexcept
		{ stream.write(colourCyan); }
	inline void lightBlue(const consoleStream_t &stream) noexcept
		{ stream.write(colourLightBlue); }
	inline void defaults(const consoleStream_t &stream) noexcept
		{ stream.write(colourDefaults); }
#else
	inline void red(const consoleStream_t &stream) noexcept
		{ SetConsoleTextAttribute(stream.handle(), FOREGROUND_RED | FOREGROUND_INTENSITY); }
	inline void yellow(const consoleStream_t &stream) noexcept
		{ SetConsoleTextAttribute(stream.handle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); }
	inline void cyan(const consoleStream_t &stream) noexcept
		{ SetConsoleTextAttribute(stream.handle(), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); }
	inline void lightBlue(const consoleStream_t &stream) noexcept
		{ SetConsoleTextAttribute(stream.handle(), FOREGROUND_BLUE | FOREGROUND_INTENSITY); }
	inline void defaults(const consoleStream_t &stream) noexcept
		{ SetConsoleTextAttribute(stream.handle(), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); }
#endif

	void console_t::_error() const noexcept
	{
		const bool tty = errorStream.isTTY();
		errorStream.write(' ');
		if (tty)
			red(errorStream);
		errorStream.write(errorPrefix);
		if (tty)
			defaults(errorStream);
		errorStream.write(' ');
	}

	void console_t::_warning() const noexcept
	{
		const bool tty = outputStream.isTTY();
		outputStream.write(' ');
		if (tty)
			yellow(outputStream);
		outputStream.write(warningPrefix);
		if (tty)
			defaults(outputStream);
		outputStream.write(' ');
	}

	void console_t::_info() const noexcept
	{
		const bool tty = outputStream.isTTY();
		outputStream.write(' ');
		if (tty)
			cyan(outputStream);
		outputStream.write(infoPrefix);
		if (tty)
			defaults(outputStream);
		outputStream.write(' ');
	}

	void console_t::_debug() const noexcept
	{
		const bool tty = outputStream.isTTY();
		outputStream.write(' ');
		if (tty)
			lightBlue(outputStream);
		outputStream.write(debugPrefix);
		if (tty)
			defaults(outputStream);
		outputStream.write(' ');
	}
} // namespace substrate
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
