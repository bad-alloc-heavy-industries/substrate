// SPDX-License-Identifier: BSD-3-Clause
#ifndef _MSC_VER
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#else
#include <Winsock2.h>
#endif
#include <cstring>
#include <substrate/socket>
#include <catch.hpp>

using substrate::socket_t;
using substrate::socketType_t;
using substrate::prepare;

template<typename T, typename U> T as(const U &value) noexcept
{
	T result{};
	static_assert(sizeof(T) <= sizeof(U), "sizeof(T) > sizeof(U)");
	memcpy(&result, &value, sizeof(T));
	return result;
}

TEST_CASE("sockaddr prepare", "[socket_t]")
{
	const auto ipv4Addr = prepare(socketType_t::ipv4, "127.0.0.1", 0);
	const auto ipv6Addr = prepare(socketType_t::ipv6, "::1", 0);

	if (ipv4Addr.ss_family != AF_UNSPEC)
	{
		REQUIRE(ipv4Addr.ss_family == AF_INET);
		const auto addr{as<sockaddr_in>(ipv4Addr)};
		REQUIRE(addr.sin_addr.s_addr == 0x0100007FU);
	}

	if (ipv6Addr.ss_family != AF_UNSPEC)
	{
		REQUIRE(ipv6Addr.ss_family == AF_INET6);
		const auto addr{as<sockaddr_in6>(ipv6Addr)};
		REQUIRE(memcmp(&addr.sin6_addr, &in6addr_loopback, sizeof(in6_addr)) == 0);
	}
}

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
