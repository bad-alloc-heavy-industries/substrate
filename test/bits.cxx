// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>

#if __cplusplus < 201402L && !defined(SUBSTRATE_CXX11_COMPAT)
#	define SUBSTRATE_CXX11_COMPAT
#endif
#include <substrate/bits>
#include <catch.hpp>

using substrate::shift_nibble;
TEST_CASE("shift nibble", "[bits]")
{
	constexpr uint64_t value{0x000000000000000FLLU};
	constexpr uint64_t value2{0x0123456789ABCDEFLLU};

	REQUIRE(shift_nibble<decltype(value)>(value, 1U) == 0x00000000000000F0LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 2U) == 0x0000000000000F00LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 3U) == 0x000000000000F000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 4U) == 0x00000000000F0000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 5U) == 0x0000000000F00000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 6U) == 0x000000000F000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 7U) == 0x00000000F0000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 8U) == 0x0000000F00000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 9U) == 0x000000F000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 10U) == 0x00000F0000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 11U) == 0x0000F00000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 12U) == 0x000F000000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 13U) == 0x00F0000000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 14U) == 0x0F00000000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 15U) == 0xF000000000000000LLU);

	REQUIRE(shift_nibble<decltype(value2)>(value2, 2U) == 0x23456789ABCDEF01LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 4U) == 0x456789ABCDEF0123LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 6U) == 0x6789ABCDEF012345LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 8U) == 0x89ABCDEF01234567LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 10U) == 0xABCDEF0123456789LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 12U) == 0xCDEF0123456789ABLLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 14U) == 0xEF0123456789ABCDLLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 16U) == 0x0123456789ABCDEFLLU);

	REQUIRE(shift_nibble<decltype(value2)>(value2, 4U) == 0x456789ABCDEF0123LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 8U) == 0x89ABCDEF01234567LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 12U) == 0xCDEF0123456789ABLLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 16U) == 0x0123456789ABCDEFLLU);
}

using substrate::swap16;
using substrate::swap32;
using substrate::swap64;
TEST_CASE("bswap tests", "[bits]")
{
	SECTION("bswap16")
	{
		REQUIRE(swap16(0x3412U) == 0x1234U);
		REQUIRE(swap16(0x1234U) == 0x3412U);
		REQUIRE(swap16(0x2413U) == 0x1324U);
		REQUIRE(swap16(0x2143U) == 0x4321U);
	}

	SECTION("bswap32")
	{
		REQUIRE(swap32(0x78563412U) == 0x12345678U);
		REQUIRE(swap32(0x12345678U) == 0x78563412U);
		REQUIRE(swap32(0x34127856U) == 0x56781234U);
		REQUIRE(swap32(0x56341278U) == 0x78123456U);
	}

	SECTION("bswap64")
	{
		REQUIRE(swap64(0xEFCDAB8967452301LLU) == 0x0123456789ABCDEFLLU);
		REQUIRE(swap64(0x0123456789ABCDEFLLU) == 0xEFCDAB8967452301LLU);
		REQUIRE(swap64(0x89ABCDEF01234567LLU) == 0x67452301EFCDAB89LLU);
		REQUIRE(swap64(0xAB8967452301EFCDLLU) == 0xCDEF0123456789ABLLU);
	}
}

using substrate::rotl;
TEST_CASE("rotl tests", "[bits]")
{

}

using substrate::rotr;
TEST_CASE("rotr tests", "[bits]")
{

}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
