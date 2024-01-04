// SPDX-License-Identifier: BSD-3-Clause

// https://sonarsource.atlassian.net/browse/CPP-2561
#ifndef _CRT_USE_BUILTIN_OFFSETOF
#define _CRT_USE_BUILTIN_OFFSETOF
#endif

#include <cstring>

#ifdef _WIN32
#	include <winsock2.h>
#else
#	include <sys/socket.h>
#	include <netdb.h>
#	include <arpa/inet.h>
#	include <netinet/in.h>
#	include <netinet/udp.h>
#	include <unistd.h>
#endif

#include <substrate/utility>
#include <substrate/socket>

#ifndef _WIN32
using substrate::INVALID_SOCKET;
inline int closesocket(const int s) { return close(s); }
#endif

using namespace substrate;

inline size_t sockaddrLen(const sockaddr_storage &addr) noexcept
{
	switch (addr.ss_family)
	{
		case AF_INET:
			return sizeof(sockaddr_in);
		case AF_INET6:
			return sizeof(sockaddr_in6);
	}
	return sizeof(sockaddr);
}

socket_t::socket_t(const int family, const int type, const int protocol) noexcept :
	socket(::socket(family, type, protocol)) { }
socket_t::~socket_t() noexcept
	{ if (socket != INVALID_SOCKET) closesocket(socket); }
bool socket_t::bind(const void *const addr, const size_t len) const noexcept
	{ return ::bind(socket, static_cast<const sockaddr *>(addr), socklen_t(len)) == 0; }
bool socket_t::bind(const sockaddr_storage &addr) const noexcept
	{ return bind(static_cast<const void *>(&addr), sockaddrLen(addr)); }
bool socket_t::connect(const void *const addr, const size_t len) const noexcept
	{ return ::connect(socket, static_cast<const sockaddr *>(addr), socklen_t(len)) == 0; }
bool socket_t::connect(const sockaddr_storage &addr) const noexcept
	{ return connect(static_cast<const void *>(&addr), sockaddrLen(addr)); }
bool socket_t::listen(const int32_t queueLength) const noexcept
	{ return ::listen(socket, queueLength) == 0; }
socket_t socket_t::accept(sockaddr *peerAddr, socklen_t *peerAddrLen) const noexcept
	{ return {::accept(socket, peerAddr, peerAddrLen)}; }
#ifndef _WIN32
ssize_t socket_t::write(const void *const bufferPtr, const size_t len) const noexcept
	{ return ::write(socket, bufferPtr, len); }
ssize_t socket_t::read(void *const bufferPtr, const size_t len) const noexcept
	{ return ::read(socket, bufferPtr, len); }
#else
ssize_t socket_t::write(const void *const bufferPtr, const size_t len) const noexcept
	{ return ::send(socket, static_cast<const char *>(bufferPtr), int32_t(len), 0); }
ssize_t socket_t::read(void *const bufferPtr, const size_t len) const noexcept
	{ return ::recv(socket, static_cast<char *>(bufferPtr), int32_t(len), 0); }
#endif

ssize_t socket_t::writeto(void *const bufferPtr, const size_t len, const sockaddr_storage &addr) const noexcept
{
	return ::sendto(socket, static_cast<char *>(bufferPtr), bufferlen_t(len), 0,
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		reinterpret_cast<const sockaddr *>(&addr), socklen_t(sockaddrLen(addr))); // lgtm[cpp/reinterpret-cast]
}

ssize_t socket_t::readfrom(void *const bufferPtr, const size_t len, sockaddr_storage &addr) const noexcept
{
	socklen_t size = sizeof(sockaddr_storage);
	return ::recvfrom(socket, static_cast<char *>(bufferPtr), bufferlen_t(len), 0,
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
		reinterpret_cast<sockaddr *>(&addr), &size); // lgtm[cpp/reinterpret-cast]
}

char socket_t::peek() const noexcept
{
	char buffer{};
	if (::recv(socket, &buffer, 1, MSG_PEEK) != 1)
		return {};
	return buffer;
}

inline uint16_t toBE(const uint16_t value) noexcept
{
	const std::array<uint8_t, 2> resultBytes
	{{
		uint8_t(value >> 8U),
		uint8_t(value)
	}};
	uint16_t result{};
	memcpy(&result, resultBytes.data(), resultBytes.size());
	return result;
}

template<size_t offset> inline void *offsetPtr(void *ptr)
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	const auto addr = reinterpret_cast<std::uintptr_t>(ptr); // lgtm[cpp/reinterpret-cast]
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	return reinterpret_cast<void *>(addr + offset); // lgtm[cpp/reinterpret-cast]
}

template<size_t offset, typename T, typename U> inline void copyToOffset(T &dest, const U value)
	{ memcpy(offsetPtr<offset>(&dest), &value, sizeof(U)); }

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
sockaddr_storage substrate::socket::prepare(const socketType_t family, const char *const where,
	const uint16_t port, const socketProtocol_t protocol) noexcept
{
#ifdef _WIN32
	const auto wVersionRequested{MAKEWORD(2, 2)};
	WSADATA wsaData{};
	const auto err{WSAStartup(wVersionRequested, &wsaData)};
	if (err)
		// No usable WinSock DLL.
		return {AF_UNSPEC};
#endif

	addrinfo hints{};
	hints.ai_family = typeToFamily(family);
	hints.ai_socktype = protocolToType(protocol);
	hints.ai_protocol = protocolToHints(protocol);
	hints.ai_flags = AI_PASSIVE; // This may not be right/complete..

	addrinfo *results = nullptr;
	const auto res = getaddrinfo(where, nullptr, &hints, &results);
	if (res || !results)
		return {AF_UNSPEC};

	sockaddr_storage service{};
	memcpy(&service, results->ai_addr, familyToSize(results->ai_addr->sa_family));
	freeaddrinfo(results);

	const auto portBE = toBE(port);
	if (service.ss_family == AF_INET)
		copyToOffset<offsetof(sockaddr_in, sin_port)>(service, portBE);
	else if (service.ss_family == AF_INET6)
		copyToOffset<offsetof(sockaddr_in6, sin6_port)>(service, portBE);
	else
		return {AF_UNSPEC};
	return service;
}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

int substrate::socket::typeToFamily(const socketType_t type) noexcept
{
	switch(type)
	{
		case socketType_t::ipv4:
			return AF_INET;
		case socketType_t::ipv6:
			return AF_INET6;
		case socketType_t::unknown:
			// fallback
		case socketType_t::dontCare:
			return AF_UNSPEC;
	}
}

int substrate::socket::protocolToHints(const socketProtocol_t protocol) noexcept
{
	switch (protocol)
	{
		case socketProtocol_t::udp:
			return IPPROTO_UDP;
		case socketProtocol_t::raw:
			return IPPROTO_RAW;
		case socketProtocol_t::tcp:
			return IPPROTO_TCP;
	}
}

int substrate::socket::protocolToType(const socketProtocol_t protocol) noexcept
{
	switch (protocol)
	{
		case socketProtocol_t::udp:
			return SOCK_DGRAM;
		case socketProtocol_t::raw:
			return SOCK_RAW;
		case socketProtocol_t::tcp:
			return SOCK_STREAM;
	}
}

size_t substrate::socket::familyToSize(sa_family_t family) noexcept
{
	if (family == AF_INET)
		return sizeof(sockaddr_in);
	else if (family == AF_INET6)
		return sizeof(sockaddr_in6);
	else
		return sizeof(sockaddr_storage);
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
