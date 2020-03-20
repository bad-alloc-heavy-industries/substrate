#include <cstdint>
#include <memory>
#include <array>
#include <substrate/fd>
#include <catch.hpp>

using substrate::fd_t;

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
}

TEST_CASE("fd_t bad open", "[fd_t]")
{
	fd_t file{"bad.file", O_RDONLY};
	REQUIRE(file == -1);
	REQUIRE_FALSE(file.valid());
	REQUIRE_FALSE(file.isEOF());
}

TEST_CASE("fd_t write", "[fd_t]")
{
	fd_t file{"fd.test", O_WRONLY | O_CREAT | O_EXCL, substrate::normalMode};
	REQUIRE(file >= 0);
	REQUIRE(file.valid());
	REQUIRE_FALSE(file.isEOF());

	REQUIRE(file.write(testArray));
	REQUIRE(file.write(testChar));
	REQUIRE(file.write(u8));
	REQUIRE(file.write(i8));
	REQUIRE(file.writeLE(u16));
	REQUIRE(file.writeLE(u32));
}

template<typename T> void read(fd_t &file, const T &expected)
{
	T result{};
	REQUIRE(file.read(result));
	REQUIRE(result == expected);
}

template<typename T> void readLE(fd_t &file, const T expected)
{
	T result{};
	REQUIRE(file.readLE(result));
	REQUIRE(result == expected);
}

TEST_CASE("fd_t read", "[fd_t]")
{
	fd_t file{"fd.test", O_RDONLY};
	REQUIRE(file >= 0);
	REQUIRE(file.valid());
	REQUIRE_FALSE(file.isEOF());

	read(file, testArray);
	read(file, testChar);
	//read(file, testString);
	read(file, u8);
	read(file, i8);
	readLE(file, u16);
	readLE(file, u32);
}

ANON_TEST_CASE()
{
	unlink("fd.test");
	SUCCEED();
}
