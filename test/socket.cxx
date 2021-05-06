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
#include <catch2/catch.hpp>

using substrate::socket_t;
using substrate::socketType_t;
using substrate::socketProtocol_t;
using substrate::socket::prepare;

template<typename T, typename U> T as(const U &value) noexcept
{
	T result{};
	static_assert(sizeof(T) <= sizeof(U), "sizeof(T) > sizeof(U)");
	memcpy(&result, &value, sizeof(T));
	return result;
}

void assertErrno(int expected)
{
	REQUIRE(errno == expected);
	errno = 0;
}

TEST_CASE("sockaddr prepare", "[socket_t]")
{
	/* default TCP */
	const auto ipv4Addr = prepare(socketType_t::ipv4, "127.0.0.1", 0);
	const auto ipv6Addr = prepare(socketType_t::ipv6, "::1", 0);
	const auto unknAddr = prepare(socketType_t::dontCare, "", 0);
	const auto dncAddr = prepare(socketType_t::dontCare, "localhost", 0);


	if (ipv4Addr.ss_family != AF_UNSPEC)
	{
		REQUIRE(ipv4Addr.ss_family == AF_INET);
		const auto addr{as<sockaddr_in>(ipv4Addr)};
		REQUIRE(addr.sin_addr.s_addr == 0x0100007FU);
		REQUIRE(addr.sin_port == 0);
	}

	if (ipv6Addr.ss_family != AF_UNSPEC)
	{
		REQUIRE(ipv6Addr.ss_family == AF_INET6);
		const auto addr{as<sockaddr_in6>(ipv6Addr)};
		REQUIRE(memcmp(&addr.sin6_addr, &in6addr_loopback, sizeof(in6_addr)) == 0);
		REQUIRE(addr.sin6_port == 0);
	}

	REQUIRE(unknAddr.ss_family == AF_UNSPEC);
	REQUIRE((dncAddr.ss_family == AF_INET || dncAddr.ss_family == AF_INET6));

	/* UDP sockets */
	const auto ipv4AddrUDP = prepare(socketType_t::ipv4, "127.0.0.1", 0, socketProtocol_t::udp);
	const auto ipv6AddrUDP = prepare(socketType_t::ipv6, "::1", 0, socketProtocol_t::udp);
	const auto unknAddrUDP = prepare(socketType_t::dontCare, "", 0, socketProtocol_t::udp);
	const auto dncAddrUDP = prepare(socketType_t::dontCare, "localhost", 0, socketProtocol_t::udp);

	if (ipv4AddrUDP.ss_family != AF_UNSPEC)
	{
		REQUIRE(ipv4AddrUDP.ss_family == AF_INET);
		const auto addr{as<sockaddr_in>(ipv4AddrUDP)};
		REQUIRE(addr.sin_addr.s_addr == 0x0100007FU);
		REQUIRE(addr.sin_port == 0);
	}

	if (ipv6AddrUDP.ss_family != AF_UNSPEC)
	{
		REQUIRE(ipv6AddrUDP.ss_family == AF_INET6);
		const auto addr{as<sockaddr_in6>(ipv6AddrUDP)};
		REQUIRE(memcmp(&addr.sin6_addr, &in6addr_loopback, sizeof(in6_addr)) == 0);
		REQUIRE(addr.sin6_port == 0);
	}

	REQUIRE(unknAddrUDP.ss_family == AF_UNSPEC);
	REQUIRE((dncAddrUDP.ss_family == AF_INET || dncAddrUDP.ss_family == AF_INET6));

	/* RAW sockets */
	const auto ipv4AddrRAW = prepare(socketType_t::ipv4, "127.0.0.1", 0, socketProtocol_t::raw);
	const auto ipv6AddrRAW = prepare(socketType_t::ipv6, "::1", 0, socketProtocol_t::raw);
	const auto unknAddrRAW = prepare(socketType_t::dontCare, "", 0, socketProtocol_t::raw);
	const auto dncAddrRAW = prepare(socketType_t::dontCare, "localhost", 0, socketProtocol_t::raw);

	if (ipv4AddrRAW.ss_family != AF_UNSPEC)
	{
		REQUIRE(ipv4AddrRAW.ss_family == AF_INET);
		const auto addr{as<sockaddr_in>(ipv4AddrRAW)};
		REQUIRE(addr.sin_addr.s_addr == 0x0100007FU);
		REQUIRE(addr.sin_port == 0);
	}

	if (ipv6AddrRAW.ss_family != AF_UNSPEC)
	{
		REQUIRE(ipv6AddrUDP.ss_family == AF_INET6);
		const auto addr{as<sockaddr_in6>(ipv6AddrRAW)};
		REQUIRE(memcmp(&addr.sin6_addr, &in6addr_loopback, sizeof(in6_addr)) == 0);
		REQUIRE(addr.sin6_port == 0);
	}

	REQUIRE(unknAddrRAW.ss_family == AF_UNSPEC);
	REQUIRE((dncAddrRAW.ss_family == AF_INET || dncAddrRAW.ss_family == AF_INET6));
}

TEST_CASE("socket_t bad socket", "[socket_t]")
{
	const auto addr = prepare(socketType_t::dontCare, "localhost", 0);
	errno = 0;
	socket_t socket{};
	REQUIRE(socket == -1);
	REQUIRE_FALSE(socket.valid());

	assertErrno(0);
	REQUIRE(socket.read(nullptr, 0) == -1);
	assertErrno(EBADF);
	REQUIRE(socket.write(nullptr, 0) == -1);
	assertErrno(EBADF);
	REQUIRE_FALSE(socket.bind(addr));
	assertErrno(EBADF);
	REQUIRE_FALSE(socket.connect(addr));
	assertErrno(EBADF);
	REQUIRE_FALSE(socket.listen(1));
	assertErrno(EBADF);
	REQUIRE(socket.peek() == 0);
	assertErrno(EBADF);
}

TEST_CASE("socket_t inheriting construction", "[socket_t]")
{
	const int32_t socketFD{::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)};
	REQUIRE(socketFD != -1);
	socket_t socket{socketFD};
	REQUIRE(socket.valid());
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
