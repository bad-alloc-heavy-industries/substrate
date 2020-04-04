// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/console>
#include <substrate/fd>
#include <catch.hpp>

#ifndef _WINDOWS
constexpr static const char *defaultTTY = "/dev/ptmx";
#else
constexpr static const char *defaultTTY = "CON";
#endif

using substrate::console;
using substrate::consoleStream_t;
using substrate::fd_t;

TEST_CASE("consoleStream_t construction", "[console_t] [!mayfail]")
{
	fd_t tty{defaultTTY, O_WRONLY | O_NOCTTY};
	REQUIRE(tty.valid());
	REQUIRE(isatty(tty));
	consoleStream_t stream{tty};
	REQUIRE(stream.valid());
	REQUIRE(stream.isTTY());
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
