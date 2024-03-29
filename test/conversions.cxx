#include <cstring>
#include <utility>
#include <vector>
#include <chrono>
#include <substrate/conversions>

#include <catch2/catch_test_macros.hpp>

using substrate::fromInt_t;
using substrate::toInt_t;

template<typename int_t> struct testOkValue_t
{
	int_t inputNumber;
	const char *variableResult;
	const char *fixedResult;
};

template<typename int_t> struct testOkHexValue_t
{
	testOkValue_t<int_t> data;
	bool upperCase;
};

template<typename int_t> using testOkPair_t = std::pair<int_t, const char *const>;
template<typename int_t> using testOk_t = std::vector<testOkValue_t<int_t>>;
template<typename int_t> using testOkHex_t = std::vector<testOkHexValue_t<int_t>>;
template<typename int_t> using testPairOk_t = std::vector<testOkPair_t<int_t>>;
template<typename int_t> using testFailInt_t = std::vector<int_t>;
using testFailStr_t = std::vector<const char *>;
using str_t = std::char_traits<char>;

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define u8(n)  static_cast<uint8_t>(n)
#define i8(n)  static_cast<int8_t>(n)
#define u16(n) static_cast<uint16_t>(n)
#define i16(n) static_cast<int16_t>(n)
#define u32(n) UINT32_C(n)
#define i32(n) INT32_C(n)
#define u64(n) UINT64_C(n)
#define i64(n) INT64_C(n)
// NOLINTEND(cppcoreguidelines-macro-usage)

template<typename> constexpr inline size_t typeToOctalLength() noexcept { return static_cast<std::size_t>(-1); }
template<> constexpr inline size_t typeToOctalLength<uint8_t>() noexcept { return 3; }
template<> constexpr inline size_t typeToOctalLength<uint16_t>() noexcept { return 6; }
template<> constexpr inline size_t typeToOctalLength<uint32_t>() noexcept { return 11; }
template<> constexpr inline size_t typeToOctalLength<uint64_t>() noexcept { return 22; }

template<typename> constexpr inline size_t typeToDecLength() noexcept;
template<> constexpr inline size_t typeToDecLength<uint8_t>() noexcept { return 4; }
template<> constexpr inline size_t typeToDecLength<uint16_t>() noexcept { return 6; }
template<> constexpr inline size_t typeToDecLength<uint32_t>() noexcept { return 11; }
template<> constexpr inline size_t typeToDecLength<uint64_t>() noexcept { return 21; }
template<> constexpr inline size_t typeToDecLength<int8_t>() noexcept { return 5; }
template<> constexpr inline size_t typeToDecLength<int16_t>() noexcept { return 7; }
template<> constexpr inline size_t typeToDecLength<int32_t>() noexcept { return 12; }
template<> constexpr inline size_t typeToDecLength<int64_t>() noexcept { return 21; }

template<typename> constexpr inline size_t typeToHexLength() noexcept { return static_cast<std::size_t>(-1); }
template<> constexpr inline size_t typeToHexLength<uint8_t>() noexcept { return 3; }
template<> constexpr inline size_t typeToHexLength<uint16_t>() noexcept { return 5; }
template<> constexpr inline size_t typeToHexLength<uint32_t>() noexcept { return 9; }
template<> constexpr inline size_t typeToHexLength<uint64_t>() noexcept { return 17; }

template<typename int_t> struct testFromInt_t final
{
private:
	using fromInt = fromInt_t<int_t, int_t>;
	using toOctalFixed = fromInt_t<int_t, int_t, typeToOctalLength<int_t>()>;
	using toDecFixed = fromInt_t<int_t, int_t, typeToDecLength<int_t>()>;
	using toHexFixed = fromInt_t<int_t, int_t, typeToHexLength<int_t>()>;

public:
	static void testOctConversions(const testOk_t<int_t> &tests)
	{
		for (const auto &test : tests)
		{
			const auto inputNumber{test.inputNumber};
			const auto result{test.variableResult};
			const auto value
			{
				[&]()
				{
					std::string resultValue{};
					REQUIRE_NOTHROW([&]()
					{
						resultValue = fromInt{inputNumber}.toOctal();
					}());
					return resultValue;
				}()
			};
			REQUIRE(!value.empty());
			REQUIRE(memcmp(value.data(), result, str_t::length(result)) == 0);
		}

		for (const auto &test : tests)
		{
			const auto inputNumber{test.inputNumber};
			const auto result{test.fixedResult};
			const auto value
			{
				[&]()
				{
					std::string resultValue{};
					REQUIRE_NOTHROW([&]()
					{
						resultValue = toOctalFixed{inputNumber}.toOctal();
					}());
					return resultValue;
				}()
			};
			REQUIRE(!value.empty());
			REQUIRE(memcmp(value.data(), result, str_t::length(result)) == 0);
		}
	}

	static void testDecConversions(const testOk_t<int_t> &tests)
	{
		for (const auto &test : tests)
		{
			const auto inputNumber{test.inputNumber};
			const auto *const result{test.variableResult};
			const std::unique_ptr<char []> value{fromInt{inputNumber}};
			REQUIRE(value.get());
			REQUIRE(memcmp(value.get(), result, str_t::length(result)) == 0);
			const std::unique_ptr<const char []> valuePtr{static_cast<const char *>(fromInt{inputNumber})};
			REQUIRE(valuePtr.get());
			REQUIRE(memcmp(valuePtr.get(), result, str_t::length(result)) == 0);
			REQUIRE_NOTHROW([&]()
			{
				const std::string valueStr{fromInt{inputNumber}};
				REQUIRE(memcmp(valueStr.data(), result, str_t::length(result)) == 0);
			}());
		}

		for (const auto &test : tests)
		{
			const auto inputNumber{test.inputNumber};
			const auto *const result{test.fixedResult};
			const std::unique_ptr<char []> value{toDecFixed{inputNumber}};
			REQUIRE(value.get());
			REQUIRE(memcmp(value.get(), result, str_t::length(result)) == 0);
			const std::unique_ptr<const char []> valuePtr{static_cast<const char *>(toDecFixed{inputNumber})};
			REQUIRE(valuePtr.get());
			REQUIRE(memcmp(valuePtr.get(), result, str_t::length(result)) == 0);
			REQUIRE_NOTHROW([&]()
			{
				const std::string valueStr{toDecFixed{inputNumber}};
				REQUIRE(memcmp(valueStr.data(), result, str_t::length(result)) == 0);
			}());
		}
	}

	static void testHexConversions(const testOkHex_t<int_t> &tests)
	{
		for (const auto &test : tests)
		{
			const auto inputNumber{test.data.inputNumber};
			const auto *const result{test.data.variableResult};
			REQUIRE_NOTHROW([&]()
			{
				const auto value{fromInt{inputNumber}.toHex(test.upperCase)};
				REQUIRE(!value.empty());
				REQUIRE(memcmp(value.data(), result, str_t::length(result)) == 0);
			}());
		}

		for (const auto &test : tests)
		{
			const auto inputNumber{test.data.inputNumber};
			const auto *const result{test.data.fixedResult};
			REQUIRE_NOTHROW([&]()
			{
				const auto value{toHexFixed{inputNumber}.toHex(test.upperCase)};
				REQUIRE(!value.empty());
				REQUIRE(memcmp(value.data(), result, str_t::length(result)) == 0);
			}());
		}
	}
};

TEST_CASE("Octal conversion from uint8_t", "[conversions]")
{
	testFromInt_t<uint8_t>::testOctConversions(
	{
		{u8(0), "0", "000"},
		{u8(7), "7", "007"},
		{u8(8), "10", "010"},
		{u8(32), "40", "040"},
		{u8(127), "177", "177"},
		{u8(128), "200", "200"},
		{u8(255), "377", "377"}
	});
}

TEST_CASE("Octal conversion from uint16_t", "[conversions]")
{
	testFromInt_t<uint16_t>::testOctConversions(
	{
		{u16(0), "0", "000000"},
		{u16(256), "400", "000400"},
		{u16(32767), "77777", "077777"},
		{u16(32768), "100000", "100000"},
		{u16(65535), "177777", "177777"}
	});
}

TEST_CASE("Octal conversion from uint32_t", "[conversions]")
{
	testFromInt_t<uint32_t>::testOctConversions(
	{
		{u32(0), "0", "00000000000"},
		{u32(65536), "200000", "00000200000"},
		{u32(2147483647), "17777777777", "17777777777"},
		{u32(2147483648), "20000000000", "20000000000"},
		{u32(4294967295), "37777777777", "37777777777"}
	});
}

TEST_CASE("Octal conversion from uint64_t", "[conversions]")
{
	testFromInt_t<uint64_t>::testOctConversions(
	{
		{u64(0), "0", "0000000000000000"},
		{u64(4294967296), "40000000000", "0000000000040000000000"},
		{u64(140737488355327), "3777777777777777", "0000003777777777777777"},
		{u64(140737488355328), "4000000000000000", "0000004000000000000000"},
		{u64(9223372036854775807), "777777777777777777777", "0777777777777777777777"},
		{u64(9223372036854775808), "1000000000000000000000", "1000000000000000000000"},
		{u64(18446744073709551615), "1777777777777777777777", "1777777777777777777777"}
	});
}

TEST_CASE("Decimal conversion from uint8_t", "[conversions]")
{
	testFromInt_t<uint8_t>::testDecConversions(
	{
		{u8(0), "0", "0000"},
		{u8(127), "127", "0127"},
		{u8(128), "128", "0128"},
		{u8(255), "255", "0255"}
	});
}

TEST_CASE("Decimal conversion from int8_t", "[conversions]")
{
	testFromInt_t<int8_t>::testDecConversions(
	{
		{i8(0), "0", "00000"},
		{i8(127), "127", "00127"},
		{i8(-1), "-1", "-0001"},
		{i8(-127), "-127", "-0127"},
		{i8(-128), "-128", "-0128"}
	});
}

TEST_CASE("Decimal conversion from uint16_t", "[conversions]")
{
	testFromInt_t<uint16_t>::testDecConversions(
	{
		{u16(0), "0", "000000"},
		{u16(127), "127", "000127"},
		{u16(128), "128", "000128"},
		{u16(255), "255", "000255"},
		{u16(256), "256", "000256"},
		{u16(32767), "32767", "032767"},
		{u16(32768), "32768", "032768"},
		{u16(65535), "65535", "065535"}
	});
}

TEST_CASE("Decimal conversion from int16_t", "[conversions]")
{
	testFromInt_t<int16_t>::testDecConversions(
	{
		{i16(0), "0", "0000000"},
		{i16(127), "127", "0000127"},
		{i16(128), "128", "0000128"},
		{i16(255), "255", "0000255"},
		{i16(256), "256", "0000256"},
		{i16(32767), "32767", "0032767"},
		{i16(-1), "-1", "-000001"},
		{i16(-127), "-127", "-000127"},
		{i16(-128), "-128", "-000128"},
		{i16(-255), "-255", "-000255"},
		{i16(-256), "-256", "-000256"},
		{i16(-32767), "-32767", "-032767"},
		{i16(-32768), "-32768", "-032768"}
	});
}

TEST_CASE("Decimal conversion from uint32_t", "[conversions]")
{
	testFromInt_t<uint32_t>::testDecConversions(
	{
		{u32(0), "0", "00000000000"},
		{u32(127), "127", "00000000127"},
		{u32(128), "128", "00000000128"},
		{u32(255), "255", "00000000255"},
		{u32(256), "256", "00000000256"},
		{u32(32767), "32767", "00000032767"},
		{u32(32768), "32768", "00000032768"},
		{u32(65535), "65535", "00000065535"},
		{u32(65536), "65536", "00000065536"},
		{u32(2147483647), "2147483647", "02147483647"},
		{u32(2147483648), "2147483648", "02147483648"},
		{u32(4294967295), "4294967295", "04294967295"}
	});
}

TEST_CASE("Decimal conversion from int32_t", "[conversions]")
{
	testFromInt_t<int32_t>::testDecConversions(
	{
		{i32(0), "0", "000000000000"},
		{i32(127), "127", "000000000127"},
		{i32(128), "128", "000000000128"},
		{i32(255), "255", "000000000255"},
		{i32(256), "256", "000000000256"},
		{i32(32767), "32767", "000000032767"},
		{i32(32768), "32768", "000000032768"},
		{i32(65535), "65535", "000000065535"},
		{i32(65536), "65536", "000000065536"},
		{i32(2147483647), "2147483647", "002147483647"},
		{i32(-1), "-1", "-00000000001"},
		{i32(-127), "-127", "-00000000127"},
		{i32(-128), "-128", "-00000000128"},
		{i32(-255), "-255", "-00000000255"},
		{i32(-256), "-256", "-00000000256"},
		{i32(-32767), "-32767", "-00000032767"},
		{i32(-32768), "-32768", "-00000032768"},
		{i32(-65535), "-65535", "-00000065535"},
		{i32(-65536), "-65536", "-00000065536"},
		{i32(-2147483647), "-2147483647", "-02147483647"},
#if __GNUC__ > 5 || _MSC_VER
		{i32(-2147483647) - 1, "-2147483648", "-02147483648"}
#endif
	});
}

TEST_CASE("Decimal conversion from uint64_t", "[conversions]")
{
	testFromInt_t<uint64_t>::testDecConversions(
	{
		{u64(0), "0", "000000000000000000000"},
		{u64(127), "127", "000000000000000000127"},
		{u64(128), "128", "000000000000000000128"},
		{u64(255), "255", "000000000000000000255"},
		{u64(256), "256", "000000000000000000256"},
		{u64(32767), "32767", "000000000000000032767"},
		{u64(32768), "32768", "000000000000000032768"},
		{u64(65535), "65535", "000000000000000065535"},
		{u64(65536), "65536", "000000000000000065536"},
		{u64(2147483647), "2147483647", "000000000002147483647"},
		{u64(2147483648), "2147483648", "000000000002147483648"},
		{u64(4294967295), "4294967295", "000000000004294967295"},
		{u64(4294967296), "4294967296", "000000000004294967296"},
		{u64(140737488355327), "140737488355327", "000000140737488355327"},
		{u64(140737488355328), "140737488355328", "000000140737488355328"},
		{u64(9223372036854775807), "9223372036854775807", "009223372036854775807"},
		{u64(9223372036854775808), "9223372036854775808", "009223372036854775808"},
		{u64(18446744073709551615), "18446744073709551615", "018446744073709551615"}
	});
}

TEST_CASE("Decimal conversion from int64_t", "[conversions]")
{
	testFromInt_t<int64_t>::testDecConversions(
	{
		{i64(0), "0", "000000000000000000000"},
		{i64(127), "127", "000000000000000000127"},
		{i64(128), "128", "000000000000000000128"},
		{i64(255), "255", "000000000000000000255"},
		{i64(256), "256", "000000000000000000256"},
		{i64(32767), "32767", "000000000000000032767"},
		{i64(32768), "32768", "000000000000000032768"},
		{i64(65535), "65535", "000000000000000065535"},
		{i64(65536), "65536", "000000000000000065536"},
		{i64(2147483647), "2147483647", "000000000002147483647"},
		{i64(2147483648), "2147483648", "000000000002147483648"},
		{i64(4294967295), "4294967295", "000000000004294967295"},
		{i64(4294967296), "4294967296", "000000000004294967296"},
		{i64(140737488355327), "140737488355327", "000000140737488355327"},
		{i64(140737488355328), "140737488355328", "000000140737488355328"},
		{i64(9223372036854775807), "9223372036854775807", "009223372036854775807"},
		{i64(-1), "-1", "-00000000000000000001"},
		{i64(-127), "-127", "-00000000000000000127"},
		{i64(-128), "-128", "-00000000000000000128"},
		{i64(-255), "-255", "-00000000000000000255"},
		{i64(-256), "-256", "-00000000000000000256"},
		{i64(-32767), "-32767", "-00000000000000032767"},
		{i64(-32768), "-32768", "-00000000000000032768"},
		{i64(-65535), "-65535", "-00000000000000065535"},
		{i64(-65536), "-65536", "-00000000000000065536"},
		{i64(-2147483647), "-2147483647", "-00000000002147483647"},
		{i64(-2147483648), "-2147483648", "-00000000002147483648"},
		{i64(-4294967295), "-4294967295", "-00000000004294967295"},
		{i64(-4294967296), "-4294967296", "-00000000004294967296"},
		{i64(-140737488355327), "-140737488355327", "-00000140737488355327"},
		{i64(-140737488355328), "-140737488355328", "-00000140737488355328"},
		{i64(-9223372036854775807), "-9223372036854775807", "-09223372036854775807"},
#if __GNUC__ > 5 || _MSC_VER
		{i64(-9223372036854775807) - 1, "-9223372036854775808", "-09223372036854775808"}
#endif
	});
}

TEST_CASE("Hexadecimal conversion from uint8_t", "[conversions]")
{
	testFromInt_t<uint8_t>::testHexConversions(
	{
		{{u8(0), "0", "000"}, true},
		{{u8(127), "7F", "07F"}, true},
		{{u8(128), "80", "080"}, true},
		{{u8(255), "FF", "0FF"}, true},
		{{u8(127), "7f", "07f"}, false},
		{{u8(170), "aa", "0aa"}, false},
		{{u8(255), "ff", "0ff"}, false}
	});
}

TEST_CASE("Hexadecimal conversion from uint16_t", "[conversions]")
{
	testFromInt_t<uint16_t>::testHexConversions(
	{
		{{u64(0), "0", "00000"}, true},
		{{u64(256), "100", "00100"}, true},
		{{u64(32767), "7FFF", "07FFF"}, true},
		{{u64(32768), "8000", "08000"}, true},
		{{u64(65535), "FFFF", "0FFFF"}, true},
		{{u64(32767), "7fff", "07fff"}, false},
		{{u64(43690), "aaaa", "0aaaa"}, false},
		{{u64(65535), "ffff", "0ffff"}, false}
	});
}

TEST_CASE("Hexadecimal conversion from uint32_t", "[conversions]")
{
	testFromInt_t<uint32_t>::testHexConversions(
	{
		{{u32(0), "0", "000000000"}, true},
		{{u32(65536), "10000", "000010000"}, true},
		{{u32(2147483647), "7FFFFFFF", "07FFFFFFF"}, true},
		{{u32(2147483648), "80000000", "080000000"}, true},
		{{u32(4294967295), "FFFFFFFF", "0FFFFFFFF"}, true},
		{{u32(2147483647), "7fffffff", "07fffffff"}, false},
		{{u32(2863311530), "aaaaaaaa", "0aaaaaaaa"}, false},
		{{u32(4294967295), "ffffffff", "0ffffffff"}, false}
	});
}

TEST_CASE("Hexadecimal conversion from uint64_t", "[conversions]")
{
	testFromInt_t<uint64_t>::testHexConversions(
	{
		{{u64(0), "0", "00000000000000000"}, true},
		{{u64(4294967296), "100000000", "00000000100000000"}, true},
		{{u64(140737488355327), "7FFFFFFFFFFF", "000007FFFFFFFFFFF"}, true},
		{{u64(140737488355328), "800000000000", "00000800000000000"}, true},
		{{u64(281474976710655), "FFFFFFFFFFFF", "00000FFFFFFFFFFFF"}, true},
		{{u64(9223372036854775807), "7FFFFFFFFFFFFFFF", "07FFFFFFFFFFFFFFF"}, true},
		{{u64(9223372036854775808), "8000000000000000", "08000000000000000"}, true},
		{{u64(18446744073709551615), "FFFFFFFFFFFFFFFF", "0FFFFFFFFFFFFFFFF"}, true},
		{{u64(140737488355327), "7fffffffffff", "000007fffffffffff"}, false},
		{{u64(187649984473770), "aaaaaaaaaaaa", "00000aaaaaaaaaaaa"}, false},
		{{u64(281474976710655), "ffffffffffff", "00000ffffffffffff"}, false},
		{{u64(9223372036854775807), "7fffffffffffffff", "07fffffffffffffff"}, false},
		{{u64(12297829382473034410), "aaaaaaaaaaaaaaaa", "0aaaaaaaaaaaaaaaa"}, false},
		{{u64(18446744073709551615), "ffffffffffffffff", "0ffffffffffffffff"}, false}
	});
}

template<typename int_t> struct testToInt_t final
{
private:
	using toInt = toInt_t<int_t>;

public:
	static void testOctConversions(const testPairOk_t<int_t> &tests)
	{
		for (const auto &test : tests)
		{
			auto value = toInt{test.second};
			REQUIRE(value.isOct());
			REQUIRE(value.length() == str_t::length(test.second));
			REQUIRE(value.fromOct() == test.first);
		}
	}

	static void testDecConversions(const testPairOk_t<int_t> &tests)
	{
		for (const auto &test : tests)
		{
			auto value = toInt{test.second};
			REQUIRE(value.isDec());
			REQUIRE(value.length() == str_t::length(test.second));
			REQUIRE(value == test.first);
			REQUIRE(value.fromDec() == test.first);
		}
	}

	static void testHexConversions(const testPairOk_t<int_t> &tests)
	{
		for (const auto &test : tests)
		{
			auto value = toInt{test.second};
			REQUIRE(value.isHex());
			REQUIRE(value.length() == str_t::length(test.second));
			REQUIRE(value.fromHex() == test.first);
		}
	}
};

template<typename int_t> struct toIntType_t
{
private:
	toInt_t<int_t> value;

public:
	toIntType_t(const char *const test) noexcept : value(test) { }
	template<typename test_t> void test(const char *const test) { test_t{test}(value); }
};

template<typename... int_t> struct toIntTypes_t;
template<typename int_t, typename... ints_t> struct toIntTypes_t<int_t, ints_t...> : toIntTypes_t<ints_t...>
{
private:
	toIntType_t<int_t> type;

public:
	toIntTypes_t(const char *const test) noexcept : toIntTypes_t<ints_t...>(test), type(test) { }

	template<template<typename> class test_t> void test(const char *const test)
	{
		type.template test<test_t<toInt_t<int_t>>>(test);
		toIntTypes_t<ints_t...>::template test<test_t>(test);
	}
};

template<> struct toIntTypes_t<>
{
	toIntTypes_t(const char *const) noexcept { }
	template<template<typename> class test_t> void test(const char *const) { }
};

TEST_CASE("Octal conversion to uint8_t", "[conversions]")
{
	testToInt_t<uint8_t>::testOctConversions(
	{
		{u8(0), ""},
		{u8(0), "0"},
		{u8(0), "00"},
		{u8(0), "000"},
		{u8(127), "177"},
		{u8(128), "200"},
		{u8(255), "377"}
	});
}

TEST_CASE("Octal conversion to int8_t", "[conversions]")
{
	testToInt_t<int8_t>::testOctConversions(
	{
		{i8(0), ""},
		{i8(0), "000"},
		{i8(127), "177"},
		{i8(-1), "377"},
		{i8(-127), "201"},
		{i8(-128), "200"}
	});
}

TEST_CASE("Octal conversion to uint16_t", "[conversions]")
{
	testToInt_t<uint16_t>::testOctConversions(
	{
		{u16(0), ""},
		{u16(0), "000000"},
		{u16(256), "000400"},
		{u16(32767), "077777"},
		{u16(32768), "100000"},
		{u16(65535), "177777"}
	});
}

TEST_CASE("Octal conversion to int16_t", "[conversions]")
{
	testToInt_t<int16_t>::testOctConversions(
	{
		{i16(0), ""},
		{i16(0), "000000"},
		{i16(128), "000200"},
		{i16(255), "000377"},
		{i16(256), "000400"},
		{i16(32767), "077777"},
		{i16(-1), "177777"},
		{i16(-255), "177401"},
		{i16(-256), "177400"},
		{i16(-32767), "100001"},
		{i16(-32768), "100000"}
	});
}

TEST_CASE("Octal conversion to uint32_t", "[conversions]")
{
	testToInt_t<uint32_t>::testOctConversions(
	{
		{u32(0), ""},
		{u32(0), "00000000000"},
		{u32(65536), "00000200000"},
		{u32(2147483647), "17777777777"},
		{u32(2147483648), "20000000000"},
		{u32(4294967295), "37777777777"}
	});
}

TEST_CASE("Octal conversion to int32_t", "[conversions]")
{
	testToInt_t<int32_t>::testOctConversions(
	{
		{i32(0), ""},
		{i32(0), "00000000000"},
		{i32(32768), "00000100000"},
		{i32(65535), "00000177777"},
		{i32(65536), "00000200000"},
		{i32(2147483647), "17777777777"},
		{i32(-1), "37777777777"},
		{i32(-65535), "37777600001"},
		{i32(-65536), "37777600000"},
		{i32(-2147483647), "20000000001"},
		{i32(-2147483647) - 1, "20000000000"}
	});
}

TEST_CASE("Octal conversion to uint64_t", "[conversions]")
{
	testToInt_t<uint64_t>::testOctConversions(
	{
		{u64(0), ""},
		{u64(0), "0000000000000000"},
		{u64(4294967296), "0000040000000000"},
		{u64(140737488355327), "0000003777777777777777"},
		{u64(140737488355328), "0000004000000000000000"},
		{u64(9223372036854775807), "0777777777777777777777"},
		{u64(9223372036854775808), "1000000000000000000000"},
		{u64(18446744073709551615), "1777777777777777777777"}
	});
}

TEST_CASE("Octal conversion to int64_t", "[conversions] ")
{
	testToInt_t<int64_t>::testOctConversions(
	{
		{i64(0), ""},
		{i64(0), "0000000000000000"},
		{i64(2147483648), "0000020000000000"},
		{i64(4294967295), "0000037777777777"},
		{i64(4294967296), "0000040000000000"},
		{i64(140737488355327), "0000003777777777777777"},
		{i64(140737488355328), "0000004000000000000000"},
		{i64(9223372036854775807), "0777777777777777777777"},
		{i64(-1), "1777777777777777777777"},
		{i64(-4294967295), "1777777777740000000001"},
		{i64(-4294967296), "1777777777740000000000"},
		{i64(-140737488355327), "1777774000000000000001"},
		{i64(-140737488355328), "1777774000000000000000"},
		{i64(-9223372036854775807), "1000000000000000000001"},
		{i64(-9223372036854775807) - 1, "1000000000000000000000"}
	});
}

template<typename toInt_t> struct testOctShouldFail_t
{
private:
	const char *test;

public:
	testOctShouldFail_t(const char *const test_) noexcept : test{test_} { }

	void operator ()(toInt_t &value)
	{
		REQUIRE(!value.isOct());
		REQUIRE(value.length() == str_t::length(test));
		REQUIRE(value.fromOct() == 0);
	}
};

void testOctShouldFail(const testFailStr_t tests)
{
	for (const char *const test : tests)
	{
		toIntTypes_t<uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t> types{test};
		types.template test<testOctShouldFail_t>(test);
	}
}

TEST_CASE("Invalid octal conversions", "[conversions]")
{
	testOctShouldFail(
	{
		"-", "+", "a", "A",
		"-a", "-A", "$", "*",
		"/", "\x01", " ", "\t",
		"\n", "\r", "^", "&",
		"8", "9", "g", "G"
	});
}

TEST_CASE("Decimal conversion to uint8_t", "[conversions]")
{
	testToInt_t<uint8_t>::testDecConversions(
	{
		{u8(0), ""},
		{u8(0), "0"},
		{u8(127), "127"},
		{u8(128), "128"},
		{u8(255), "255"}
	});
}

TEST_CASE("Decimal conversion to int8_t", "[conversions]")
{
	testToInt_t<int8_t>::testDecConversions(
	{
		{i8(0), ""},
		{i8(0), "0"},
		{i8(127), "127"},
		{i8(-1), "-1"},
		{i8(-127), "-127"},
		{i8(-128), "-128"}
	});
}

TEST_CASE("Decimal conversion to uint16_t", "[conversions]")
{
	testToInt_t<uint16_t>::testDecConversions(
	{
		{u16(0), ""},
		{u16(0), "0"},
		{u16(256), "256"},
		{u16(32767), "32767"},
		{u16(32768), "32768"},
		{u16(65535), "65535"}
	});
}

TEST_CASE("Decimal conversion to int16_t", "[conversions]")
{
	testToInt_t<int16_t>::testDecConversions(
	{
		{i16(0), ""},
		{i16(0), "0"},
		{i16(128), "128"},
		{i16(255), "255"},
		{i16(256), "256"},
		{i16(32767), "32767"},
		{i16(-1), "-1"},
		{i16(-255), "-255"},
		{i16(-256), "-256"},
		{i16(-32767), "-32767"},
		{i16(-32768), "-32768"}
	});
}

TEST_CASE("Decimal conversion to uint32_t", "[conversions]")
{
	testToInt_t<uint32_t>::testDecConversions(
	{
		{u32(0), ""},
		{u32(0), "0"},
		{u32(65536), "65536"},
		{u32(2147483647), "2147483647"},
		{u32(2147483648), "2147483648"},
		{u32(4294967295), "4294967295"}
	});
}

TEST_CASE("Decimal conversion to int32_t", "[conversions]")
{
	testToInt_t<int32_t>::testDecConversions(
	{
		{i32(0), ""},
		{i32(0), "0"},
		{i32(32768), "32768"},
		{i32(65535), "65535"},
		{i32(65536), "65536"},
		{i32(2147483647), "2147483647"},
		{i32(-1), "-1"},
		{i32(-65535), "-65535"},
		{i32(-65536), "-65536"},
		{i32(-2147483647), "-2147483647"},
		{i32(-2147483647) - 1, "-2147483648"}
	});
}

TEST_CASE("Decimal conversion to uint64_t", "[conversions]")
{
	testToInt_t<uint64_t>::testDecConversions(
	{
		{u64(0), ""},
		{u64(0), "0"},
		{u64(4294967296), "4294967296"},
		{u64(140737488355327), "140737488355327"},
		{u64(140737488355328), "140737488355328"},
		{u64(9223372036854775807), "9223372036854775807"},
		{u64(9223372036854775808), "9223372036854775808"},
		{u64(18446744073709551615), "18446744073709551615"}
	});
}

TEST_CASE("Decimal conversion to int64_t", "[conversions]")
{
	testToInt_t<int64_t>::testDecConversions(
	{
		{i64(0), ""},
		{i64(0), "0"},
		{i64(2147483648), "2147483648"},
		{i64(4294967295), "4294967295"},
		{i64(4294967296), "4294967296"},
		{i64(140737488355327), "140737488355327"},
		{i64(140737488355328), "140737488355328"},
		{i64(9223372036854775807), "9223372036854775807"},
		{i64(-1), "-1"},
		{i64(-4294967295), "-4294967295"},
		{i64(-4294967296), "-4294967296"},
		{i64(-140737488355327), "-140737488355327"},
		{i64(-140737488355328), "-140737488355328"},
		{i64(-9223372036854775807), "-9223372036854775807"},
		{i64(-9223372036854775807) - 1, "-9223372036854775808"}
	});
}

template<typename toInt_t> struct testDecShouldFail_t
{
private:
	const char *test;

public:
	testDecShouldFail_t(const char *const test_) noexcept : test{test_} { }

	void operator ()(toInt_t &value)
	{
		REQUIRE(!value.isDec());
		REQUIRE(value.length() == str_t::length(test));
		REQUIRE(value == 0);
		REQUIRE(value.fromDec() == 0);
	}
};

void testDecShouldFail(const testFailStr_t tests)
{
	for (const char *const test : tests)
	{
		toIntTypes_t<uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t> types{test};
		types.template test<testDecShouldFail_t>(test);
	}
}

TEST_CASE("Invalid decimal conversions", "[conversions]")
{
	testDecShouldFail(
	{
		"-", "+", "a", "A",
		"-a", "-A", "$", "*",
		"/", "\x01", " ", "\t",
		"\n", "\r", "^", "&",
		"g", "G"
	});
}

TEST_CASE("Hexadecimal conversion to uint8_t", "[conversions]")
{
	testToInt_t<uint8_t>::testHexConversions(
	{
		{u8(0), ""},
		{u8(0), "0"},
		{u8(0), "00"},
		{u8(127), "7F"},
		{u8(128), "80"},
		{u8(255), "FF"},
		{u8(127), "7f"},
		{u8(170), "aa"},
		{u8(255), "ff"}
	});
}

TEST_CASE("Hexadecimal conversion to int8_t", "[conversions]")
{
	testToInt_t<int8_t>::testHexConversions(
	{
		{i8(0), ""},
		{i8(0), "00"},
		{i8(127), "7F"},
		{i8(-1), "FF"},
		{i8(-127), "81"},
		{i8(-128), "80"}
	});
}

TEST_CASE("Hexadecimal conversion to uint16_t", "[conversions]")
{
	testToInt_t<uint16_t>::testHexConversions(
	{
		{u16(0), ""},
		{u16(0), "00"},
		{u16(256), "0100"},
		{u16(32767), "7FFF"},
		{u16(32768), "8000"},
		{u16(65535), "FFFF"}
	});
}

TEST_CASE("Hexadecimal conversion to int16_t", "[conversions]")
{
	testToInt_t<int16_t>::testHexConversions(
	{
		{i16(0), ""},
		{i16(0), "00"},
		{i16(128), "80"},
		{i16(255), "FF"},
		{i16(256), "0100"},
		{i16(32767), "7FFF"},
		{i16(-1), "FFFF"},
		{i16(-255), "FF01"},
		{i16(-256), "FF00"},
		{i16(-32767), "8001"},
		{i16(-32768), "8000"}
	});
}

TEST_CASE("Hexadecimal conversion to uint32_t", "[conversions]")
{
	testToInt_t<uint32_t>::testHexConversions(
	{
		{u32(0), ""},
		{u32(0), "00"},
		{u32(65536), "00010000"},
		{u32(2147483647), "7FFFFFFF"},
		{u32(2147483648), "80000000"},
		{u32(4294967295), "FFFFFFFF"}
	});
}

TEST_CASE("Hexadecimal conversion to int32_t", "[conversions]")
{
	testToInt_t<int32_t>::testHexConversions(
	{
		{i32(0), ""},
		{i32(0), "00"},
		{i32(32768), "8000"},
		{i32(65535), "FFFF"},
		{i32(65536), "00010000"},
		{i32(2147483647), "7FFFFFFF"},
		{i32(-1), "FFFFFFFF"},
		{i32(-65535), "FFFF0001"},
		{i32(-65536), "FFFF0000"},
		{i32(-2147483647), "80000001"},
		{i32(-2147483647) - 1, "80000000"}
	});
}

TEST_CASE("Hexadecimal conversion to uint64_t", "[conversions]")
{
	testToInt_t<uint64_t>::testHexConversions(
	{
		{u64(0), ""},
		{u64(0), "00"},
		{u64(4294967296), "0000000100000000"},
		{u64(140737488355327), "00007FFFFFFFFFFF"},
		{u64(140737488355328), "0000800000000000"},
		{u64(9223372036854775807), "7FFFFFFFFFFFFFFF"},
		{u64(9223372036854775808), "8000000000000000"},
		{u64(18446744073709551615), "FFFFFFFFFFFFFFFF"}
	});
}

TEST_CASE("Hexadecimal conversion to int64_t", "[conversions]")
{
	testToInt_t<int64_t>::testHexConversions(
	{
		{i64(0), ""},
		{i64(0), "00"},
		{i64(2147483648), "80000000"},
		{i64(4294967295), "FFFFFFFF"},
		{i64(4294967296), "0000000100000000"},
		{i64(140737488355327), "00007FFFFFFFFFFF"},
		{i64(140737488355328), "0000800000000000"},
		{i64(9223372036854775807), "7FFFFFFFFFFFFFFF"},
		{i64(-1), "FFFFFFFFFFFFFFFF"},
		{i64(-4294967295), "FFFFFFFF00000001"},
		{i64(-4294967296), "FFFFFFFF00000000"},
		{i64(-140737488355327), "FFFF800000000001"},
		{i64(-140737488355328), "FFFF800000000000"},
		{i64(-9223372036854775807), "8000000000000001"},
		{i64(-9223372036854775807) - 1, "8000000000000000"}
	});
}

template<typename toInt_t> struct testHexShouldFail_t
{
private:
	const char *test;

public:
	testHexShouldFail_t(const char *const test_) noexcept : test{test_} { }

	void operator ()(toInt_t &value)
	{
		REQUIRE(!value.isHex());
		REQUIRE(value.length() == str_t::length(test));
		REQUIRE(value.fromHex() == 0);
	}
};

void testHexShouldFail(const testFailStr_t tests)
{
	for (const char *const test : tests)
	{
		toIntTypes_t<uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t> types{test};
		types.template test<testHexShouldFail_t>(test);
	}
}

TEST_CASE("Invalid hexadecimal conversions", "[conversions]")
{
	testHexShouldFail(
	{
		"-", "+", "g", "G",
		"-a", "-A", "$", "*",
		"/", "\x01", " ", "\t",
		"\n", "\r", "^", "&"
	});
}

constexpr std::chrono::nanoseconds operator ""_ns(const unsigned long long value) noexcept
	{ return std::chrono::nanoseconds{value}; }

TEST_CASE("Fractional component conversions", "[conversions]")
{
	using nanoseconds = std::chrono::nanoseconds;
	using fromIntVariable = fromInt_t<typename nanoseconds::rep, nanoseconds>;
	using fromIntFixed = fromInt_t<typename nanoseconds::rep, nanoseconds, 9>;

	const auto checkResult
	{
		[](const char *const expected, const char *result)
		{
			REQUIRE(expected);
			REQUIRE(result);
			const auto expectedLength{str_t::length(expected)};
			const auto resultLength{str_t::length(result)};
			REQUIRE(expectedLength == resultLength);
			REQUIRE(memcmp(result, expected, expectedLength) == 0);
		}
	};

	const auto testConversion
	{
		[&](const nanoseconds value, const char *const variableExpected, const char *const fixedExpected)
		{
			const auto variableResult{fromIntVariable{value}.formatFraction(21)};
			checkResult(variableExpected, variableResult.get());

			const auto fixedResult{fromIntFixed{value}.formatFraction(9)};
			checkResult(fixedExpected, fixedResult.get());
		}
	};

	testConversion(0_ns, "0", "0");
	testConversion(900900_ns, "0000000000000009009", "0009009");
	testConversion(123456789123000_ns, "000000123456789123", "789123");

	const auto uint32Value{static_cast<uint32_t>(876543210U)};
	const auto uint32{substrate::fromInt(uint32Value)};
	const auto uint32Result{uint32.formatFraction(12)};
	checkResult("00087654321", uint32Result.get());
}
