// SPDX-License-Identifier: BSD-3-Clause
#include <cstring>
#include <memory>
#include <array>
#include <substrate/fd>
#include <substrate/utility>
#include <catch.hpp>

using substrate::fd_t;
using substrate::make_unique;

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

TEST_CASE("fd_t bad fd", "[fd_t]")
{
	fd_t file{};
	REQUIRE(file == -1);
	REQUIRE_FALSE(file.valid());
	REQUIRE_FALSE(file.isEOF());

	REQUIRE(file.read(nullptr, 0, nullptr) == -1);
	REQUIRE(file.write(nullptr, 0, nullptr) == -1);
	REQUIRE_FALSE(file.tail());
}

TEST_CASE("fd_t bad open", "[fd_t]")
{
	fd_t file{"bad.file", O_RDONLY};
	REQUIRE(file == -1);
	REQUIRE_FALSE(file.valid());
	REQUIRE_FALSE(file.isEOF());
}

TEST_CASE("fd_t std::string open", "[fd_t]")
{
	std::string filename{"bad.file"};
	fd_t file{filename, O_RDONLY};
	REQUIRE(file == -1);
	REQUIRE_FALSE(file.valid());
	REQUIRE_FALSE(file.isEOF());
}

static std::unique_ptr<char []> toUnique(const std::string &value)
{
	auto result{make_unique<char []>(value.size())};
	memcpy(result.get(), value.data(), value.size());
	return result;
}

static std::unique_ptr<char> toUnique(const char value)
{
	auto result{make_unique<char>()};
	*result = value;
	return result;
}

TEST_CASE("fd_t write", "[fd_t]")
{
	fd_t file{"fd.test", O_WRONLY | O_CREAT | O_EXCL, substrate::normalMode};
	REQUIRE(file >= 0);
	REQUIRE(file.valid());
	REQUIRE_FALSE(file.isEOF());

	REQUIRE(file.write(testArray));
	REQUIRE(file.write(testChar));
	auto arrPtr = toUnique(testString);
	REQUIRE(file.write(arrPtr, testString.size()));
	auto objPtr = toUnique(testChar);
	REQUIRE(file.write(objPtr));
	REQUIRE(file.write(u8));
	REQUIRE(file.write(i8));
	REQUIRE(file.writeLE(u16));
	REQUIRE(file.writeLE(i16));
	REQUIRE(file.writeLE(u32));
	REQUIRE(file.writeLE(i32));
	REQUIRE(file.writeLE(u64));
	REQUIRE(file.writeLE(i64));
	REQUIRE(file.writeBE(u16));
	REQUIRE(file.writeBE(i16));
	REQUIRE(file.writeBE(u32));
	REQUIRE(file.writeBE(i32));
	REQUIRE(file.writeBE(u64));
	REQUIRE(file.writeBE(i64));
}

TEST_CASE("fd_t seek", "[fd_t]")
{
	fd_t file{"fd.test", O_RDONLY};
	REQUIRE(file.valid());
	REQUIRE(file.tell() == 0);
	const off_t length = file.length();
	REQUIRE(length == 78);
	REQUIRE(file.tail());
	REQUIRE(file.tell() == length);
	REQUIRE(file.seek(-(length / 2), SEEK_CUR) == length / 2);
	REQUIRE(file.seek(0, SEEK_END) == length);
	REQUIRE(file.head());
	REQUIRE_FALSE(file.isEOF());
}

static void readUnique(const fd_t &file, const std::string &expected)
{
	auto result{make_unique<char []>(expected.size())};
	REQUIRE(result != nullptr);
	REQUIRE(file.read(result, expected.size()));
	REQUIRE(memcmp(result.get(), expected.data(), expected.size()) == 0);
	REQUIRE_FALSE(file.isEOF());
}

static void readUnique(const fd_t &file, const char expected)
{
	auto result{make_unique<char>()};
	REQUIRE(result != nullptr);
	REQUIRE(file.read(result));
	REQUIRE(*result == expected);
	REQUIRE_FALSE(file.isEOF());
}

template<typename T> static void read(const fd_t &file, const T &expected)
{
	T result{};
	REQUIRE(file.read(result));
	REQUIRE(result == expected);
	REQUIRE_FALSE(file.isEOF());
}

template<typename T> static void readLE(const fd_t &file, const T expected)
{
	T result{};
	REQUIRE(file.readLE(result));
	REQUIRE(result == expected);
	REQUIRE_FALSE(file.isEOF());
}

template<typename T> static void readBE(const fd_t &file, const T expected)
{
	T result{};
	REQUIRE(file.readBE(result));
	REQUIRE(result == expected);
	REQUIRE_FALSE(file.isEOF());
}

TEST_CASE("fd_t read", "[fd_t]")
{
	fd_t file{"fd.test", O_RDONLY};
	REQUIRE(file >= 0);
	REQUIRE(file.valid());
	REQUIRE_FALSE(file.isEOF());

	read(file, testArray);
	read(file, testChar);
	readUnique(file, testString);
	readUnique(file, testChar);
	read(file, u8);
	read(file, i8);
	readLE(file, u16);
	readLE(file, i16);
	readLE(file, u32);
	readLE(file, i32);
	readLE(file, u64);
	readLE(file, i64);
	readBE(file, u16);
	readBE(file, i16);
	readBE(file, u32);
	readBE(file, i32);
	readBE(file, u64);
	readBE(file, i64);

	char junk{};
	REQUIRE_FALSE(file.read(junk));
	REQUIRE(file.isEOF());
	REQUIRE_FALSE(file.read(junk));
	REQUIRE(file.isEOF());
}

ANON_TEST_CASE()
{
	unlink("fd.test");
	SUCCEED();
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
