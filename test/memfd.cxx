// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <memory>
#include <array>
#include <substrate/memfd>
#include <catch.hpp>

using substrate::memfd_t;

constexpr static std::array<char, 4> testArray{'t', 'E', 'S', 't'};
constexpr static char testChar{'.'};
static std::string testString{"fileDescriptor"};

constexpr static auto u8{uint8_t(0x5A)};
constexpr static auto i8{int8_t(0xA5)};
constexpr static auto u16{uint16_t(0x125A)};
constexpr static auto i16{int16_t(0x12A5)};
constexpr static auto u32{uint32_t(UINT32_C(0x1234565A))};
constexpr static auto i32{int32_t(INT32_C(0x123456A5))};
constexpr static auto u64{uint64_t(UINT64_C(0x123456789ABCDE5A))};
constexpr static auto i64{int64_t(INT64_C(0x123456789ABCDEA5))};

memfd_t<4096> file{"memfd.test", O_RDWR};

TEST_CASE("memfd_t bad fd", "[memfd_t]")
{
	memfd_t<4096> bad_file{};
	REQUIRE(bad_file == -1);
	REQUIRE_FALSE(bad_file.valid());
	REQUIRE_FALSE(bad_file.isEOF());

	REQUIRE(bad_file.read(nullptr, 0, nullptr) == -1);
	REQUIRE(bad_file.write(nullptr, 0, nullptr) == -1);
}


TEST_CASE("memfd_t write", "[memfd_t]")
{
	REQUIRE(file >= 0);
	REQUIRE(file.valid());
	REQUIRE_FALSE(file.isEOF());

	REQUIRE(file.write(testArray));
	REQUIRE(file.write(testChar));
	REQUIRE(file.write(u8));
	REQUIRE(file.write(i8));
	REQUIRE(file.writeLE(u16));
	REQUIRE(file.writeLE(u32));
	REQUIRE(file.writeLE(u64));
	REQUIRE(file.writeBE(u16));
	REQUIRE(file.writeBE(u32));
	REQUIRE(file.writeBE(u64));
}

TEST_CASE("memfd_t seek", "[memfd_t]")
{
	REQUIRE(file.head());
	REQUIRE(file.tail());
}

template<typename T> void read(memfd_t<4096> &file, const T &expected)
{
	T result{};
	REQUIRE(file.read(result));
	REQUIRE(result == expected);
}

template<typename T> void readLE(memfd_t<4096> &file, const T expected)
{
	T result{};
	REQUIRE(file.readLE(result));
	REQUIRE(result == expected);
}

template<typename T> void readBE(memfd_t<4096> &file, const T expected)
{
	T result{};
	REQUIRE(file.readBE(result));
	REQUIRE(result == expected);
}

TEST_CASE("memfd_t read", "[memfd_t]")
{
	REQUIRE(file >= 0);
	REQUIRE(file.valid());
	REQUIRE_FALSE(file.isEOF());
	REQUIRE(file.seek(0, SEEK_SET) == 0);
	read(file, testArray);
	read(file, testChar);
	//read(file, testString);
	read(file, u8);
	read(file, i8);
	readLE(file, u16);
	readLE(file, u32);
	readLE(file, u64);
	readBE(file, u16);
	readBE(file, u32);
	readBE(file, u64);
}
