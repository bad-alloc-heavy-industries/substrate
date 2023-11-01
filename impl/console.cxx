// SPDX-License-Identifier: BSD-3-Clause
#include <cerrno>
#ifndef _WIN32
#	include <unistd.h>
#else
#	ifndef NOMINMAX
#	define NOMINMAX
#	endif
#	ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#	endif
#	include <windows.h>
#	include <fcntl.h>
#	include <io.h>
#	include <substrate/utility>
#endif
#include <substrate/console>

using substrate::operator ""_s;
using charTraits = std::char_traits<char>;
#ifdef _WIN32
using wcharTraits = std::char_traits<wchar_t>;
#endif
using char16Traits = std::char_traits<char16_t>;

static const std::string errorPrefix{"[ERR]"_s};
static const std::string warningPrefix{"[WRN]"_s};
static const std::string infoPrefix{"[INF]"_s};
static const std::string debugPrefix{"[DBG]"_s};

#ifndef _WIN32
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
#ifndef _WIN32
		SUBSTRATE_NOWARN_UNUSED(const auto result) = ::write(fd, buffer, bufferLen);
#else
		SUBSTRATE_NOWARN_UNUSED(const auto result) = ::write(fd, buffer, uint32_t(bufferLen));
#endif
		errno = 0; // extra insurance.
	}

	void consoleStream_t::write(const char *const value) const noexcept
		{ write(value, value ? charTraits::length(value) : 0U); }

	// WARNING: This assumes you're giving it a TEXT stream so no non-printable stuff you want to preserve.
	// It will (if necessary) automatically UTF-8 => 16 convert whatever passes through for the sake of windows
	void consoleStream_t::write(const char *const value, const size_t valueLen) const noexcept
	{
		if (value)
		{
#ifdef _WIN32
			// If there's nothing to convert (0-length string), fast-exit doing nothing.
			if (!valueLen)
				return;
			const auto consoleMode{_setmode(fd, _O_U8TEXT)};
			const auto stringLen{static_cast<size_t>(MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED | MB_USEGLYPHCHARS,
				value, int(valueLen), nullptr, 0))};
			// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
			auto string{make_unique_nothrow<wchar_t []>(stringLen)};
			if (!string)
				return;
			MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED | MB_USEGLYPHCHARS, value, int(valueLen),
				string.get(), int(stringLen));
			write(static_cast<const void *>(string.get()), sizeof(wchar_t) * stringLen);
			_setmode(fd, consoleMode);
#else
			write(static_cast<const void *>(value), valueLen);
#endif
		}
		else
			write(nullString);
	}

#ifdef _WIN32
	void consoleStream_t::write(const wchar_t *const value) const noexcept
		{ write(value, value ? wcharTraits::length(value) : 0U); }

	void consoleStream_t::write(const wchar_t *const value, const size_t valueLen) const noexcept
	{
		if (value)
		{
			// If there's nothing to convert (0-length string), fast-exit doing nothing.
			if (!valueLen)
				return;
			const auto consoleMode{_setmode(fd, _O_WTEXT)};
			write(static_cast<const void *>(value), sizeof(wchar_t) * valueLen);
			_setmode(fd, consoleMode);
		}
		else
			write(nullString);
	}
#endif

	void consoleStream_t::write(const char16_t *const value) const noexcept
		{ write(value, value ? char16Traits::length(value) : 0U); }

	void consoleStream_t::write(const char16_t *const value, const size_t valueLen) const noexcept
	{
		if (value)
		{
			// If there's nothing to convert (0-length string), fast-exit doing nothing.
			if (!valueLen)
				return;
#ifdef _WIN32
			const auto consoleMode{_setmode(fd, _O_U16TEXT)};
			write(static_cast<const void *>(value), sizeof(char16_t) * valueLen);
			_setmode(fd, consoleMode);
#else
			convertingWrite(value, valueLen);
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

#ifndef _WIN32
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

	// These functions implement streamed UTF-16 to UTF-8 conversion for display
#ifndef _WIN32
	// Performs safe indexing into the string array, returning the invalid value 0xffff if outside the bounds of the array
	static inline uint16_t safeIndex(const char16_t *const string, const size_t length, const size_t index) noexcept
	{
		if (index >= length)
			return UINT16_MAX;
		uint16_t result{};
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		memcpy(&result, string + index, sizeof(char16_t));
		return result;
	}

	// Convert and stream out the converted code points given by string to the fd
	void consoleStream_t::convertingWrite(const char16_t *string, const size_t stringLen) const noexcept
	{
		for (size_t offset = 0; offset < stringLen; ++offset)
		{
			const auto unitA{safeIndex(string, stringLen, offset)};
			// Check if this is a high-half surrogate pair
			if ((unitA & 0xfe00U) == 0xd800U)
			{
				// Recover the upper 10 (11) bits from the first surrogate of the pair.
				const auto upper{(unitA & 0x03ffU) + 0x0040U};
				// Recover the lower 10 bits from the second surrogate of the pair.
				const auto lower{safeIndex(string, stringLen, ++offset) & 0x03fffU};

				// Encode the 4 code units and write them out
				write(static_cast<char>(0xf0U | (uint8_t(upper >> 8U) & 0x07U)));
				write(static_cast<char>(0x80U | (uint8_t(upper >> 2U) & 0x3fU)));
				write(static_cast<char>(0x80U | (uint8_t(upper << 4U) & 0x30U) | (uint8_t(lower >> 6U) & 0x0fU)));
				write(static_cast<char>(0x80U | (lower & 0x3fU)));
			}
			else
			{
				// Something from the Basic Multilingual Plane, figure out how to encode it.
				// If it's able to be represented as a single byte, write it out as one
				if (unitA <= 0x007fU)
					write(static_cast<char>(unitA));
				// If it's two byte, encode and write out as a code unit pair
				else if (unitA <= 0x07ffU)
				{
					write(static_cast<char>(0xc0U | (uint8_t(unitA >> 6U) & 0x1fU)));
					write(static_cast<char>(0x80U | uint8_t(unitA & 0x3fU)));
				}
				// Otherwise it's 3 byte, encode and write out as a code unit tripple
				else
				{
					write(static_cast<char>(0xe0U | (uint8_t(unitA >> 12U) & 0x0fU)));
					write(static_cast<char>(0x80U | (uint8_t(unitA >> 6U) & 0x3fU)));
					write(static_cast<char>(0x80U | (unitA & 0x3fU)));
				}
			}
		}
	}
#endif
} // namespace substrate
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
