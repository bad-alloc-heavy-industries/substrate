// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/mmap>
#include <substrate/memfd>

#include <catch.hpp>

#include <iostream>
#include <cerrno>
#include <cstring>

using substrate::mmap_t;
using substrate::memfd_t;

TEST_CASE( "Anonymous map test", "[mmap_t]")
{
	memfd_t<4096> mapfd{"mapfd", O_RDWR};
	mmap_t map{mapfd, mapfd.length(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS};
	REQUIRE(map.valid());
}
