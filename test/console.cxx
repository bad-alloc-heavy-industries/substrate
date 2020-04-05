// SPDX-License-Identifier: BSD-3-Clause
#include <thread>
#include <chrono>
#include <substrate/console>
#include <substrate/fixed_vector>
#include <substrate/fd>
#include <substrate/pty>
#include <substrate/pipe>
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
using substrate::pipe_t;
using substrate::readPipe_t;
using substrate::fixedVector_t;

const std::chrono::microseconds operator ""_us(unsigned long long us) noexcept
	{ return std::chrono::microseconds{us}; }

const std::string testString{"test"_s};
const std::string colourDebugTest{" \033[1;34m[DBG]\033[0m test\r\n"_s};
const std::string colourInfoTest{" \033[36m[INF]\033[0m test\r\n"_s};
const std::string colourWarningTest{" \033[1;33m[WRN]\033[0m test\r\n"_s};
const std::string colourErrorTest{" \033[1;31m[ERR]\033[0m test\r\n"_s};

const std::string plainDebugTest{" [DBG] test\n"_s};
const std::string plainInfoTest{" [INF] test\n"_s};
const std::string plainWarningTest{" [WRN] test\n"_s};
const std::string plainErrorTest{" [ERR] test\n"_s};

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
	std::this_thread::sleep_for(1_us);
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
	REQUIRE(console.valid());

	console.debug(testString);
	assertConsoleRead(pty.ptmx(), colourDebugTest);
	console.info(testString);
	assertConsoleRead(pty.ptmx(), colourInfoTest);
	console.warning(testString);
	assertConsoleRead(pty.ptmx(), colourWarningTest);
	console.warn(testString);
	assertConsoleRead(pty.ptmx(), colourWarningTest);
	console.error(testString);
	assertConsoleRead(pty.ptmx(), colourErrorTest);

	console = {};
	REQUIRE_FALSE(console.valid());
}
#endif

void assertPipeRead(const readPipe_t &fd, const std::string &expected)
{
	fixedVector_t<char> result{expected.length()};
	REQUIRE(result.valid());
	REQUIRE(fd.read(result.data(), result.size()));
	REQUIRE(memcmp(result.data(), expected.data(), expected.length()) == 0);
}

TEST_CASE("console_t pipe write", "[console_t]")
{
	pipe_t pipe{};
	REQUIRE(pipe.valid());
	// Initialise console_t with a fresh outputStream + errorStream
	// set to our pipe's write side
	console = {pipe.writeFD(), pipe.writeFD()};
	REQUIRE(console.valid());

	console.debug(testString);
	assertPipeRead(pipe, plainDebugTest);
	console.info(testString);
	assertPipeRead(pipe, plainInfoTest);
	console.warning(testString);
	assertPipeRead(pipe, plainWarningTest);
	console.warn(testString);
	assertPipeRead(pipe, plainWarningTest);
	console.error(testString);
	assertPipeRead(pipe, plainErrorTest);

	console = {};
	REQUIRE_FALSE(console.valid());
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
