// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/socket>
#include <catch.hpp>

using substrate::socket_t;

TEST_CASE("socket_t bad socket", "[socket_t]")
{
	socket_t socket{};
	REQUIRE(socket == -1);
	REQUIRE_FALSE(socket.valid());

	REQUIRE(socket.read(nullptr, 0) == -1);
	REQUIRE(socket.write(nullptr, 0) == -1);
	REQUIRE(socket.peek() == 0);
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
