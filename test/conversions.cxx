#include <utility>
#include <vector>
#include <cstring>
#include <substrate/conversions>

#include <catch.hpp>

using substrate::fromInt_t;
using substrate::toInt_t;

template<typename int_t> using testOkPair_t = std::pair<int_t, const char *const>;
template<typename int_t> using testOk_t = std::vector<testOkPair_t<int_t>>;
template<typename int_t> using testFailInt_t = std::vector<int_t>;
using testFailStr_t = std::vector<const char *>;
using str_t = std::char_traits<char>;

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define u64(n)		UINT64_C(n)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define i64(n)		INT64_C(n)

template<typename int_t> struct testFromInt_t final
{
private:
	using fromInt = fromInt_t<int_t, int_t>;

public:
	static void testConversions(const testOk_t<int_t> &tests)
	{
		for (const auto &test : tests)
		{
			const std::unique_ptr<char []> value{fromInt{test.first}};
			REQUIRE(value.get());
			REQUIRE(memcmp(value.get(), test.second, str_t::length(test.second)) == 0);
			const std::unique_ptr<const char []> valuePtr{static_cast<const char *>(fromInt{test.first})};
			REQUIRE(valuePtr.get());
			REQUIRE(memcmp(valuePtr.get(), test.second, str_t::length(test.second)) == 0);
			REQUIRE_NOTHROW([&]()
			{
				const std::string valueStr{fromInt{test.first}};
				REQUIRE(memcmp(valueStr.data(), test.second, str_t::length(test.second)) == 0);
			}());
		}
	}
};

TEST_CASE("Decimal conversion from uint8_t", "[conversions]")
{
	testFromInt_t<uint8_t>::testConversions(
	{
		{0, "0"},
		{127, "127"},
		{128, "128"},
		{255, "255"}
	});
}

TEST_CASE("Decimal conversion from int8_t", "[conversions]")
{
	testFromInt_t<int8_t>::testConversions(
	{
		{0, "0"},
		{127, "127"},
		{-1, "-1"},
		{-127, "-127"},
		{-128, "-128"}
	});
}

TEST_CASE("Decimal conversion from uint16_t", "[conversions]")
{
	testFromInt_t<uint16_t>::testConversions(
	{
		{0, "0"},
		{127, "127"},
		{128, "128"},
		{255, "255"},
		{256, "256"},
		{32767, "32767"},
		{32768, "32768"},
		{65535, "65535"}
	});
}

TEST_CASE("Decimal conversion from int16_t", "[conversions]")
{
	testFromInt_t<int16_t>::testConversions(
	{
		{0, "0"},
		{127, "127"},
		{128, "128"},
		{255, "255"},
		{256, "256"},
		{32767, "32767"},
		{-1, "-1"},
		{-127, "-127"},
		{-128, "-128"},
		{-255, "-255"},
		{-256, "-256"},
		{-32767, "-32767"},
		{-32768, "-32768"}
	});
}

TEST_CASE("Decimal conversion from uint32_t", "[conversions]")
{
	testFromInt_t<uint32_t>::testConversions(
	{
		{0, "0"},
		{127, "127"},
		{128, "128"},
		{255, "255"},
		{256, "256"},
		{32767, "32767"},
		{32768, "32768"},
		{65535, "65535"},
		{65536, "65536"},
		{2147483647, "2147483647"},
		{2147483648, "2147483648"},
		{4294967295, "4294967295"}
	});
}

TEST_CASE("Decimal conversion from int32_t", "[conversions]")
{
	testFromInt_t<int32_t>::testConversions(
	{
		{0, "0"},
		{127, "127"},
		{128, "128"},
		{255, "255"},
		{256, "256"},
		{32767, "32767"},
		{32768, "32768"},
		{65535, "65535"},
		{65536, "65536"},
		{2147483647, "2147483647"},
		{-1, "-1"},
		{-127, "-127"},
		{-128, "-128"},
		{-255, "-255"},
		{-256, "-256"},
		{-32767, "-32767"},
		{-32768, "-32768"},
		{-65535, "-65535"},
		{-65536, "-65536"},
		{-2147483647, "-2147483647"},
#if __GNUC__ > 5
		{-2147483648, "-2147483648"}
#endif
	});
}

TEST_CASE("Decimal conversion from uint64_t", "[conversions]")
{
	testFromInt_t<uint64_t>::testConversions(
	{
		{0, "0"},
		{127, "127"},
		{128, "128"},
		{255, "255"},
		{256, "256"},
		{32767, "32767"},
		{32768, "32768"},
		{65535, "65535"},
		{65536, "65536"},
		{2147483647, "2147483647"},
		{2147483648, "2147483648"},
		{4294967295, "4294967295"},
		{4294967296, "4294967296"},
		{140737488355327, "140737488355327"},
		{140737488355328, "140737488355328"},
		{9223372036854775807, "9223372036854775807"},
		{UINT64_C(9223372036854775808), "9223372036854775808"},
		{u64(18446744073709551615), "18446744073709551615"}
	});
}

TEST_CASE("Decimal conversion from int64_t", "[conversions]")
{
	testFromInt_t<int64_t>::testConversions(
	{
		{0, "0"},
		{127, "127"},
		{128, "128"},
		{255, "255"},
		{256, "256"},
		{32767, "32767"},
		{32768, "32768"},
		{65535, "65535"},
		{65536, "65536"},
		{2147483647, "2147483647"},
		{2147483648, "2147483648"},
		{4294967295, "4294967295"},
		{4294967296, "4294967296"},
		{140737488355327, "140737488355327"},
		{140737488355328, "140737488355328"},
		{9223372036854775807, "9223372036854775807"},
		{-1, "-1"},
		{-127, "-127"},
		{-128, "-128"},
		{-255, "-255"},
		{-256, "-256"},
		{-32767, "-32767"},
		{-32768, "-32768"},
		{-65535, "-65535"},
		{-65536, "-65536"},
		{-2147483647, "-2147483647"},
		{-2147483648, "-2147483648"},
		{-4294967295, "-4294967295"},
		{-4294967296, "-4294967296"},
		{-140737488355327, "-140737488355327"},
		{-140737488355328, "-140737488355328"},
		{-9223372036854775807, "-9223372036854775807"},
#if __GNUC__ > 5
		{i64(-9223372036854775807) - 1, "-9223372036854775808"}
#endif
	});
}

template<typename int_t> struct testToInt_t final
{
private:
	using toInt = toInt_t<int_t>;

public:
	static void testOctConversions(const testOk_t<int_t> &tests)
	{
		for (const auto &test : tests)
		{
			auto value = toInt{test.second};
			REQUIRE(value.isOct());
			REQUIRE(value.length() == str_t::length(test.second));
			REQUIRE(value.fromOct() == test.first);
		}
	}

	static void testDecConversions(const testOk_t<int_t> &tests)
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

	static void testHexConversions(const testOk_t<int_t> &tests)
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
		{0, ""},
		{0, "0"},
		{0, "00"},
		{0, "000"},
		{127, "177"},
		{128, "200"},
		{255, "377"}
	});
}

TEST_CASE("Octal conversion to int8_t", "[conversions]")
{
	testToInt_t<int8_t>::testOctConversions(
	{
		{0, ""},
		{0, "000"},
		{127, "177"},
		{-1, "377"},
		{-127, "201"},
		{-128, "200"}
	});
}

TEST_CASE("Octal conversion to uint16_t", "[conversions]")
{
	testToInt_t<uint16_t>::testOctConversions(
	{
		{0, ""},
		{0, "000000"},
		{256, "000400"},
		{32767, "077777"},
		{32768, "100000"},
		{65535, "177777"}
	});
}

TEST_CASE("Octal conversion to int16_t", "[conversions]")
{
	testToInt_t<int16_t>::testOctConversions(
	{
		{0, ""},
		{0, "000000"},
		{128, "000200"},
		{255, "000377"},
		{256, "000400"},
		{32767, "077777"},
		{-1, "177777"},
		{-255, "177401"},
		{-256, "177400"},
		{-32767, "100001"},
		{-32768, "100000"}
	});
}

TEST_CASE("Octal conversion to uint32_t", "[conversions]")
{
	testToInt_t<uint32_t>::testOctConversions(
	{
		{0, ""},
		{0, "00000000000"},
		{65536, "00000200000"},
		{2147483647, "17777777777"},
		{2147483648, "20000000000"},
		{4294967295, "37777777777"}
	});
}

TEST_CASE("Octal conversion to int32_t", "[conversions]")
{
	testToInt_t<int32_t>::testOctConversions(
	{
		{0, ""},
		{0, "00000000000"},
		{32768, "00000100000"},
		{65535, "00000177777"},
		{65536, "00000200000"},
		{2147483647, "17777777777"},
		{-1, "37777777777"},
		{-65535, "37777600001"},
		{-65536, "37777600000"},
		{-2147483647, "20000000001"},
		{-2147483648, "20000000000"}
	});
}

TEST_CASE("Octal conversion to uint64_t", "[conversions]")
{
	testToInt_t<uint64_t>::testOctConversions(
	{
		{0, ""},
		{0, "0000000000000000"},
		{4294967296, "0000040000000000"},
		{140737488355327, "0000003777777777777777"},
		{140737488355328, "0000004000000000000000"},
		{9223372036854775807, "0777777777777777777777"},
		{u64(9223372036854775808), "1000000000000000000000"},
		{u64(18446744073709551615), "1777777777777777777777"}
	});
}

TEST_CASE("Octal conversion to int64_t", "[conversions]")
{
	testToInt_t<int64_t>::testOctConversions(
	{
		{0, ""},
		{0, "0000000000000000"},
		{2147483648, "0000020000000000"},
		{4294967295, "0000037777777777"},
		{4294967296, "0000040000000000"},
		{140737488355327, "0000003777777777777777"},
		{140737488355328, "0000004000000000000000"},
		{9223372036854775807, "0777777777777777777777"},
		{-1, "1777777777777777777777"},
		{-4294967295, "1777777777740000000001"},
		{-4294967296, "1777777777740000000000"},
		{-140737488355327, "1777774000000000000001"},
		{-140737488355328, "1777774000000000000000"},
		{-9223372036854775807, "1000000000000000000001"},
		{i64(-9223372036854775807) - 1, "1000000000000000000000"}
	});
}

template<typename toInt_t> struct testOctShouldFail_t
{
private:
	const char *const test;

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
		{0, ""},
		{0, "0"},
		{127, "127"},
		{128, "128"},
		{255, "255"}
	});
}

TEST_CASE("Decimal conversion to int8_t", "[conversions]")
{
	testToInt_t<int8_t>::testDecConversions(
	{
		{0, ""},
		{0, "0"},
		{127, "127"},
		{-1, "-1"},
		{-127, "-127"},
		{-128, "-128"}
	});
}

TEST_CASE("Decimal conversion to uint16_t", "[conversions]")
{
	testToInt_t<uint16_t>::testDecConversions(
	{
		{0, ""},
		{0, "0"},
		{256, "256"},
		{32767, "32767"},
		{32768, "32768"},
		{65535, "65535"}
	});
}

TEST_CASE("Decimal conversion to int16_t", "[conversions]")
{
	testToInt_t<int16_t>::testDecConversions(
	{
		{0, ""},
		{0, "0"},
		{128, "128"},
		{255, "255"},
		{256, "256"},
		{32767, "32767"},
		{-1, "-1"},
		{-255, "-255"},
		{-256, "-256"},
		{-32767, "-32767"},
		{-32768, "-32768"}
	});
}

TEST_CASE("Decimal conversion to uint32_t", "[conversions]")
{
	testToInt_t<uint32_t>::testDecConversions(
	{
		{0, ""},
		{0, "0"},
		{65536, "65536"},
		{2147483647, "2147483647"},
		{2147483648, "2147483648"},
		{4294967295, "4294967295"}
	});
}

TEST_CASE("Decimal conversion to int32_t", "[conversions]")
{
	testToInt_t<int32_t>::testDecConversions(
	{
		{0, ""},
		{0, "0"},
		{32768, "32768"},
		{65535, "65535"},
		{65536, "65536"},
		{2147483647, "2147483647"},
		{-1, "-1"},
		{-65535, "-65535"},
		{-65536, "-65536"},
		{-2147483647, "-2147483647"},
		{-2147483648, "-2147483648"}
	});
}

TEST_CASE("Decimal conversion to uint64_t", "[conversions]")
{
	testToInt_t<uint64_t>::testDecConversions(
	{
		{0, ""},
		{0, "0"},
		{4294967296, "4294967296"},
		{140737488355327, "140737488355327"},
		{140737488355328, "140737488355328"},
		{9223372036854775807, "9223372036854775807"},
		{u64(9223372036854775808), "9223372036854775808"},
		{u64(18446744073709551615), "18446744073709551615"}
	});
}

TEST_CASE("Decimal conversion to int64_t", "[conversions]")
{
	testToInt_t<int64_t>::testDecConversions(
	{
		{0, ""},
		{0, "0"},
		{2147483648, "2147483648"},
		{4294967295, "4294967295"},
		{4294967296, "4294967296"},
		{140737488355327, "140737488355327"},
		{140737488355328, "140737488355328"},
		{9223372036854775807, "9223372036854775807"},
		{-1, "-1"},
		{-4294967295, "-4294967295"},
		{-4294967296, "-4294967296"},
		{-140737488355327, "-140737488355327"},
		{-140737488355328, "-140737488355328"},
		{-9223372036854775807, "-9223372036854775807"},
		{i64(-9223372036854775807) - 1, "-9223372036854775808"}
	});
}

template<typename toInt_t> struct testDecShouldFail_t
{
private:
	const char *const test;

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
		{0, ""},
		{0, "0"},
		{0, "00"},
		{127, "7F"},
		{128, "80"},
		{255, "FF"},
		{127, "7f"},
		{170, "aa"},
		{255, "ff"}
	});
}

TEST_CASE("Hexadecimal conversion to int8_t", "[conversions]")
{
	testToInt_t<int8_t>::testHexConversions(
	{
		{0, ""},
		{0, "00"},
		{127, "7F"},
		{-1, "FF"},
		{-127, "81"},
		{-128, "80"}
	});
}

TEST_CASE("Hexadecimal conversion to uint16_t", "[conversions]")
{
	testToInt_t<uint16_t>::testHexConversions(
	{
		{0, ""},
		{0, "00"},
		{256, "0100"},
		{32767, "7FFF"},
		{32768, "8000"},
		{65535, "FFFF"}
	});
}

TEST_CASE("Hexadecimal conversion to int16_t", "[conversions]")
{
	testToInt_t<int16_t>::testHexConversions(
	{
		{0, ""},
		{0, "00"},
		{128, "80"},
		{255, "FF"},
		{256, "0100"},
		{32767, "7FFF"},
		{-1, "FFFF"},
		{-255, "FF01"},
		{-256, "FF00"},
		{-32767, "8001"},
		{-32768, "8000"}
	});
}

TEST_CASE("Hexadecimal conversion to uint32_t", "[conversions]")
{
	testToInt_t<uint32_t>::testHexConversions(
	{
		{0, ""},
		{0, "00"},
		{65536, "00010000"},
		{2147483647, "7FFFFFFF"},
		{2147483648, "80000000"},
		{4294967295, "FFFFFFFF"}
	});
}

TEST_CASE("Hexadecimal conversion to int32_t", "[conversions]")
{
	testToInt_t<int32_t>::testHexConversions(
	{
		{0, ""},
		{0, "00"},
		{32768, "8000"},
		{65535, "FFFF"},
		{65536, "00010000"},
		{2147483647, "7FFFFFFF"},
		{-1, "FFFFFFFF"},
		{-65535, "FFFF0001"},
		{-65536, "FFFF0000"},
		{-2147483647, "80000001"},
		{-2147483648, "80000000"}
	});
}

TEST_CASE("Hexadecimal conversion to uint64_t", "[conversions]")
{
	testToInt_t<uint64_t>::testHexConversions(
	{
		{0, ""},
		{0, "00"},
		{4294967296, "0000000100000000"},
		{140737488355327, "00007FFFFFFFFFFF"},
		{140737488355328, "0000800000000000"},
		{9223372036854775807, "7FFFFFFFFFFFFFFF"},
		{u64(9223372036854775808), "8000000000000000"},
		{u64(18446744073709551615), "FFFFFFFFFFFFFFFF"}
	});
}

TEST_CASE("Hexadecimal conversion to int64_t", "[conversions]")
{
	testToInt_t<int64_t>::testHexConversions(
	{
		{0, ""},
		{0, "00"},
		{2147483648, "80000000"},
		{4294967295, "FFFFFFFF"},
		{4294967296, "0000000100000000"},
		{140737488355327, "00007FFFFFFFFFFF"},
		{140737488355328, "0000800000000000"},
		{9223372036854775807, "7FFFFFFFFFFFFFFF"},
		{-1, "FFFFFFFFFFFFFFFF"},
		{-4294967295, "FFFFFFFF00000001"},
		{-4294967296, "FFFFFFFF00000000"},
		{-140737488355327, "FFFF800000000001"},
		{-140737488355328, "FFFF800000000000"},
		{-9223372036854775807, "8000000000000001"},
		{i64(-9223372036854775807) - 1, "8000000000000000"}
	});
}

template<typename toInt_t> struct testHexShouldFail_t
{
private:
	const char *const test;

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

void testShouldFail()
{
	testHexShouldFail(
	{
		"-", "+", "g", "G",
		"-a", "-A", "$", "*",
		"/", "\x01", " ", "\t",
		"\n", "\r", "^", "&"
	});
}
