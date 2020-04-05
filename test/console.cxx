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
#else
constexpr static const char *defaultTTY = "CON";
#endif

using substrate::console;
using substrate::consoleStream_t;
using substrate::asHex_t;
using substrate::operator ""_s;
using substrate::fd_t;
using substrate::pty_t;
using substrate::pipe_t;
using substrate::readPipe_t;
using substrate::fixedVector_t;

std::chrono::microseconds operator ""_us(unsigned long long us) noexcept
	{ return std::chrono::microseconds{us}; }

struct fileFree_t
{
	void operator()(FILE *const file) const noexcept { fclose(file); } // NOLINT(cppcoreguidelines-owning-memory)
};

const std::string testString{"test"_s};
const std::string colourDebugTest{" \033[1;34m[DBG]\033[0m test\r\n"_s};
const std::string colourInfoTest{" \033[36m[INF]\033[0m test\r\n"_s};
const std::string colourWarningTest{" \033[1;33m[WRN]\033[0m test\r\n"_s};
const std::string colourErrorTest{" \033[1;31m[ERR]\033[0m test\r\n"_s};

const std::string plainDebugTest{" [DBG] test\n"_s};
const std::string plainInfoTest{" [INF] test\n"_s};
const std::string plainWarningTest{" [WRN] test\n"_s};
const std::string plainErrorTest{" [ERR] test\n"_s};

const std::string plainTrue{" [INF] true\n"_s};
const std::string plainFalse{" [INF] false\n"_s};
const std::string plainRawString{" [INF] raw string\n"_s};
const std::string plainNullptr{" [INF] (null)\n"_s};
const std::string plainChar{" [INF] 1\n"_s};
const std::string plainIntegerSigned8Min{" [INF] -128\n"_s};
const std::string plainIntegerSigned8{" [INF] -127\n"_s};
const std::string plainIntegerSigned8Max{" [INF] 127\n"_s};
const std::string plainIntegerUnsigned8{" [INF] 255\n"_s};
const std::string plainIntegerUnsigned32{" [INF] 65536\n"_s};
const std::string plainIntegerHex{" [INF] F00DFACE\n"_s};
const std::string plainIntegerPaddedHex{" [INF] 01A5\n"_s};
const std::string plainIntegerZeroHex{" [INF] ___0\n"_s};
const std::string plainIntegerOverMaxPaddedHex{" [INF] __F0\n"_s};
const std::string plainCharArray{" [INF] testing\n"_s};
const std::string plainIntegerArray{" [INF] F00D, DEAD\n"_s};

const std::array<char, 7> testCharArray{'t', 'e', 's', 't', 'i', 'n', 'g'};
const std::array<uint16_t, 2> testIntegerArray{0xF00DU, 0xDEADU};

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
	std::this_thread::sleep_for(10_us);
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

TEST_CASE("console_t write conversions", "[console_t]")
{
	pipe_t pipe{};
	REQUIRE(pipe.valid());
	std::unique_ptr<FILE, fileFree_t> file{fdopen(dup(pipe.writeFD()), "wb")};
	REQUIRE(file);
	// Initialise console_t with a fresh outputStream + errorStream
	// set to our pipe's write side
	console = {file.get(), file.get()};
	REQUIRE(console.valid());

	std::unique_ptr<char> testCharPtr{};
	REQUIRE(!testCharPtr);
	std::unique_ptr<char []> testArrayPtr{};
	REQUIRE(!testArrayPtr);

	console.info(true);
	assertPipeRead(pipe, plainTrue);
	console.info(false);
	assertPipeRead(pipe, plainFalse);
	console.info("raw string");
	assertPipeRead(pipe, plainRawString);
	console.info(testCharPtr);
	assertPipeRead(pipe, plainNullptr);
	console.info(testArrayPtr);
	assertPipeRead(pipe, plainNullptr);
	console.info('1');
	assertPipeRead(pipe, plainChar);
	console.info(int8_t{-127});
	assertPipeRead(pipe, plainIntegerSigned8);
	console.info(int8_t{-128});
	assertPipeRead(pipe, plainIntegerSigned8Min);
	console.info(int8_t{127});
	assertPipeRead(pipe, plainIntegerSigned8Max);
	console.info(uint8_t{255});
	assertPipeRead(pipe, plainIntegerUnsigned8);
	console.info(uint32_t{65536});
	assertPipeRead(pipe, plainIntegerUnsigned32);
	console.info(asHex_t<>{0xF00DFACEU});
	assertPipeRead(pipe, plainIntegerHex);
	console.info(asHex_t<4, '0'>{0x1A5U});
	assertPipeRead(pipe, plainIntegerPaddedHex);
	console.info(asHex_t<4, '_'>{0U});
	assertPipeRead(pipe, plainIntegerZeroHex);
	console.info(asHex_t<4, '_'>{uint8_t{0xF0U}});
	assertPipeRead(pipe, plainIntegerOverMaxPaddedHex);
	console.info(testCharArray);
	assertPipeRead(pipe, plainCharArray);
	console.info(testIntegerArray);
	assertPipeRead(pipe, plainIntegerArray);

	console = {};
	REQUIRE_FALSE(console.valid());
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
