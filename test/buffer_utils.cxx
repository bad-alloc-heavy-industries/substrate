// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <substrate/buffer_utils>
#include <catch2/catch.hpp>

constexpr static std::array<uint8_t, 2> x16LE{{0xF5U, 0xFFU}};
constexpr static std::array<uint8_t, 4> x32LE{{0xFAU, 0xFFU, 0xFFU, 0xFFU}};
constexpr static std::array<uint8_t, 8> x64LE{{0x00U, 0xF0U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU}};

constexpr static int16_t value16 = -11;
constexpr static int32_t value32 = -6;
constexpr static int64_t value64 = -4096;

constexpr static std::array<uint8_t, 2> x16BE{{0xFFU, 0xF5U}};
constexpr static std::array<uint8_t, 4> x32BE{{0xFFU, 0xFFU, 0xFFU, 0xFAU}};
constexpr static std::array<uint8_t, 8> x64BE{{0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xF0U, 0x00U}};

using namespace substrate;

TEST_CASE("readLE", "[buffer_utils]")
{
	REQUIRE(buffer_utils::readLE<uint16_t>(x16LE.data()) == 0xFFF5U);
	REQUIRE(buffer_utils::readLE<int16_t>(x16LE.data()) == value16);
	REQUIRE(buffer_utils::readLE<uint32_t>(x32LE.data()) == 0xFFFFFFFAU);
	REQUIRE(buffer_utils::readLE<int32_t>(x32LE.data()) == value32);
	REQUIRE(buffer_utils::readLE<uint64_t>(x64LE.data()) == UINT64_C(0xFFFFFFFFFFFFF000));
	REQUIRE(buffer_utils::readLE<int64_t>(x64LE.data()) == value64);
}

TEST_CASE("writeLE", "[buffer_utils]")
{
	std::array<uint8_t, 8> buffer{};
	buffer_utils::writeLE(value16, buffer.data());
	REQUIRE(memcmp(buffer.data(), x16LE.data(), x16LE.size()) == 0);
	buffer_utils::writeLE(value32, buffer.data());
	REQUIRE(memcmp(buffer.data(), x32LE.data(), x32LE.size()) == 0);
	buffer_utils::writeLE(value64, buffer.data());
	REQUIRE(memcmp(buffer.data(), x64LE.data(), x64LE.size()) == 0);
}

TEST_CASE("readBE", "[buffer_utils]")
{
	REQUIRE(buffer_utils::readBE<uint16_t>(x16BE.data()) == 0xFFF5U);
	REQUIRE(buffer_utils::readBE<int16_t>(x16BE.data()) == value16);
	REQUIRE(buffer_utils::readBE<uint32_t>(x32BE.data()) == 0xFFFFFFFAU);
	REQUIRE(buffer_utils::readBE<int32_t>(x32BE.data()) == value32);
	REQUIRE(buffer_utils::readBE<uint64_t>(x64BE.data()) == UINT64_C(0xFFFFFFFFFFFFF000));
	REQUIRE(buffer_utils::readBE<int64_t>(x64BE.data()) == value64);
}

TEST_CASE("writeBE", "[buffer_utils]")
{
	std::array<uint8_t, 8> buffer{};
	buffer_utils::writeBE(value16, buffer.data());
	REQUIRE(memcmp(buffer.data(), x16BE.data(), x16BE.size()) == 0);
	buffer_utils::writeBE(value32, buffer.data());
	REQUIRE(memcmp(buffer.data(), x32BE.data(), x32BE.size()) == 0);
	buffer_utils::writeBE(value64, buffer.data());
	REQUIRE(memcmp(buffer.data(), x64BE.data(), x64BE.size()) == 0);
}
