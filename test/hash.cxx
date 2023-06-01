// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>

#if __cplusplus < 201402L && !defined(SUBSTRATE_CXX11_COMPAT)
#	define SUBSTRATE_CXX11_COMPAT
#endif
#include <substrate/hash>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("murmur128", "[hash]")
{
	const std::string empty{};
	const std::array<uint8_t, 1> zero{{0x0}};
	const std::string example{"The quick brown fox jumps over the lazy dog"};
	const std::string example2{"The quick brown fox jumps over the lazy dog."};

	static constexpr auto seed{UINT32_C(0x900DBEEF)};

	const auto pair1{substrate::murmur128(empty, seed)};
	const auto pair2{substrate::murmur128(zero, seed)};
	const auto pair3{substrate::murmur128(example, seed)};
	const auto pair4{substrate::murmur128(example2, seed)};

	REQUIRE(pair1.first == UINT64_C(0xD80062720A4A93EB));
	REQUIRE(pair1.second == UINT64_C(0x9F540094625E42E2));
	REQUIRE(pair2.first == UINT64_C(0x4F31A8059DA6FD7B));
	REQUIRE(pair2.second == UINT64_C(0xF2A28C958B8932F3));
	REQUIRE(pair3.first == UINT64_C(0x48AF88C9CEE696BE));
	REQUIRE(pair3.second == UINT64_C(0x56088E21F181226E));
	REQUIRE(pair4.first == UINT64_C(0x912746B2BA9DE05));
	REQUIRE(pair4.second == UINT64_C(0x9041FFD456E73DE0));
}

TEST_CASE("bsd", "[hash]")
{
	const std::string empty{};
	const std::array<uint8_t, 1> zero{{0x0}};
	const std::string example{"The quick brown fox jumps over the lazy dog"};
	const std::string example2{"The quick brown fox jumps over the lazy dog."};

	REQUIRE(substrate::bsd_checksum(empty) == 0x0);
	REQUIRE(substrate::bsd_checksum(zero) == 0x0);
	REQUIRE(substrate::bsd_checksum(example) == 0xC56E);
	REQUIRE(substrate::bsd_checksum(example2) == 0x62E5);
}

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

TEST_CASE("crc32", "[hash]")
{
	// https://github.com/emn178/js-crc/blob/master/README.md
	const std::string empty{};
	const std::array<uint8_t, 1> zero{{0x0}};
	const std::string example{"The quick brown fox jumps over the lazy dog"};
	const std::string example2{"The quick brown fox jumps over the lazy dog."};

	REQUIRE(substrate::crc32(empty) == UINT32_C(0x00000000));
	REQUIRE(substrate::crc32(zero) == UINT32_C(0xD202EF8D));
	REQUIRE(substrate::crc32(example) == UINT32_C(0x414FA339));
	REQUIRE(substrate::crc32(example2) == UINT32_C(0x519025E9));
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
