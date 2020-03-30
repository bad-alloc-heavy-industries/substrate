// SPDX-License-Identifier: BSD-3-Clause
#include <cerrno>
#ifndef _WINDOWS
#include <unistd.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stringapiset.h>
#include <fcntl.h>
#include <io.h>
#undef WIN32_LEAN_AND_MEAN
#include <substrate/utility>
#endif
#include <substrate/console>

using namespace substrate;
using charTraits = std::char_traits<char>;

console_t console;
static const std::string errorPrefix = "[ERR]"_s;
static const std::string infoPrefix = "[INF]"_s;
static const std::string debugPrefix = "[DBG]"_s;

void consoleStream_t::checkTTY() noexcept { _tty = isatty(fd); }

void consoleStream_t::write(const void *const buffer, const size_t bufferLen) const noexcept
{
	// We don't actually care if this succeeds. We just try if at all possible.
#ifndef _WINDOWS
	[[maybe_unused]] const auto result = ::write(fd, buffer, bufferLen);
#else
	[[maybe_unused]] const auto result = ::write(fd, buffer, uint32_t(bufferLen));
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
		auto string = makeUnique<wchar_t []>(stringLen);
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
		write("(null)"_s);
}

void consoleStream_t::write(const bool value) const noexcept
{
	if (value)
		write("true"_s);
	else
		write("false"_s);
}

console_t::console_t(FILE *const outStream, FILE *const errStream) noexcept :
	outputStream{fileno(outStream)}, errorStream{fileno(errStream)},
	valid{outputStream.valid() && errorStream.valid()} { }

void console_t::_error() const noexcept
{
#ifndef _WINDOWS
	const bool tty = errorStream.isTTY();
	errorStream.write(' ');
	if (tty)
		errorStream.write("\033[1;31m"_s);
	errorStream.write(errorPrefix);
	if (tty)
		errorStream.write("\033[0m"_s);
	errorStream.write(' ');
#else
#endif
}

void console_t::_info() const noexcept
{
#ifndef _WINDOWS
	const bool tty = outputStream.isTTY();
	outputStream.write(' ');
	if (tty)
		outputStream.write("\033[36m"_s);
	outputStream.write(infoPrefix);
	if (tty)
		outputStream.write("\033[0m"_s);
	outputStream.write(' ');
#else
#endif
}

void console_t::_debug() const noexcept
{
#ifndef _WINDOWS
	const bool tty = outputStream.isTTY();
	outputStream.write(' ');
	if (tty)
		outputStream.write("\033[1;34m"_s);
	outputStream.write(debugPrefix);
	if (tty)
		outputStream.write("\033[0m"_s);
	outputStream.write(' ');
#else
#endif
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
