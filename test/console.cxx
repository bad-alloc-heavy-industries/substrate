// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/console>
#include <substrate/fixed_vector>
#include <substrate/fd>
#include <substrate/pty>
#include <catch.hpp>

#ifndef _WINDOWS
constexpr static const char *defaultTTY = "/dev/ptmx";
#	define NIX_ONLY ""
#else
constexpr static const char *defaultTTY = "CON";
#endif

using substrate::console;
using substrate::consoleStream_t;
using substrate::operator ""_s;
using substrate::fd_t;
using substrate::pty_t;
using substrate::fixedVector_t;

const std::string testString{"test"_s};
const std::string colourInfoTest{" \033[36m[INF]\033[0m test\r\n"_s};
const std::string colourDebugTest{" \033[1;34m[DBG]\033[0m test\r\n"_s};
const std::string colourErrorTest{" \033[1;31m[ERR]\033[0m test\r\n"_s};

TEST_CASE("consoleStream_t construction", "[console_t] [!mayfail]")
{
	fd_t tty{defaultTTY, O_WRONLY | O_NOCTTY};
	REQUIRE(tty.valid());
	REQUIRE(isatty(tty));
	consoleStream_t stream{tty};
	REQUIRE(stream.valid());
	REQUIRE(stream.isTTY());
}

#ifndef _WINDOWS
void assertConsoleRead(const fd_t &fd, const std::string &expected)
{
	fixedVector_t<char> result{expected.length()};
	REQUIRE(result.valid());
	REQUIRE(fd.read(result.data(), result.size()));
	REQUIRE(memcmp(result.data(), expected.data(), expected.length()) == 0);
}

TEST_CASE("console_t PTY write", "[console_t]")
{
	pty_t pty{};
	REQUIRE(pty.valid());
	// Initialise console_t with a fresh outputStream + errorStream
	// set to our new PTY's "slave" side
	console = {pty.pts(), pty.pts()};

	console.info(testString);
	assertConsoleRead(pty.ptmx(), colourInfoTest);
	console.debug(testString);
	assertConsoleRead(pty.ptmx(), colourDebugTest);
	console.error(testString);
	assertConsoleRead(pty.ptmx(), colourErrorTest);
}
#endif
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
