// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>

#if __cplusplus < 201402L && !defined(SUBSTRATE_CXX11_COMPAT)
#	define SUBSTRATE_CXX11_COMPAT
#endif
#include <substrate/hash>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("sysv", "[hash]")
{
	const std::string empty{};
	const std::array<uint8_t, 1> zero{{0x0}};
	const std::string example{"The quick brown fox jumps over the lazy dog"};
	const std::string example2{"The quick brown fox jumps over the lazy dog."};

	REQUIRE(substrate::sysv_checksum(empty) == 0x0);
	REQUIRE(substrate::sysv_checksum(zero) == 0x0);
	REQUIRE(substrate::sysv_checksum(example) == 0xFD9);
	REQUIRE(substrate::sysv_checksum(example2) == 0x1007);
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
