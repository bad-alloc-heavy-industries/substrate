#include <utility>
#include <vector>
#include <cstring>
#include <substrate/conversions>

#include <catch.hpp>

using substrate::fromInt_t;
using substrate::toInt_t;

template<typename int_t> struct testOkValue_t
{
	int_t inputNumber;
	const char *const variableResult;
	const char *const fixedResult;
};

template<typename int_t> struct testOkHexValue_t
{
	testOkValue_t<int_t> data;
	const bool upperCase;
};

template<typename int_t> using testOkPair_t = std::pair<int_t, const char *const>;
template<typename int_t> using testOk_t = std::vector<testOkValue_t<int_t>>;
template<typename int_t> using testOkHex_t = std::vector<testOkHexValue_t<int_t>>;
template<typename int_t> using testPairOk_t = std::vector<testOkPair_t<int_t>>;
template<typename int_t> using testFailInt_t = std::vector<int_t>;
using testFailStr_t = std::vector<const char *>;
using str_t = std::char_traits<char>;

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define u64(n)		UINT64_C(n)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define i64(n)		INT64_C(n)

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

template<typename int_t> struct testFromInt_t final
{
private:
	using fromInt = fromInt_t<int_t, int_t>;
	using toDecFixed = fromInt_t<int_t, int_t, typeToDecLength<int_t>()>;
	using toHexFixed = fromInt_t<int_t, int_t, typeToHexLength<int_t>()>;

public:
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

TEST_CASE("Decimal conversion from uint8_t", "[conversions]")
{
	testFromInt_t<uint8_t>::testDecConversions(
	{
		{0, "0", "0000"},
		{127, "127", "0127"},
		{128, "128", "0128"},
		{255, "255", "0255"}
	});
}

TEST_CASE("Decimal conversion from int8_t", "[conversions]")
{
	testFromInt_t<int8_t>::testDecConversions(
	{
		{0, "0", "00000"},
		{127, "127", "00127"},
		{-1, "-1", "-0001"},
		{-127, "-127", "-0127"},
		{-128, "-128", "-0128"}
	});
}

TEST_CASE("Decimal conversion from uint16_t", "[conversions]")
{
	testFromInt_t<uint16_t>::testDecConversions(
	{
		{0, "0", "000000"},
		{127, "127", "000127"},
		{128, "128", "000128"},
		{255, "255", "000255"},
		{256, "256", "000256"},
		{32767, "32767", "032767"},
		{32768, "32768", "032768"},
		{65535, "65535", "065535"}
	});
}

TEST_CASE("Decimal conversion from int16_t", "[conversions]")
{
	testFromInt_t<int16_t>::testDecConversions(
	{
		{0, "0", "0000000"},
		{127, "127", "0000127"},
		{128, "128", "0000128"},
		{255, "255", "0000255"},
		{256, "256", "0000256"},
		{32767, "32767", "0032767"},
		{-1, "-1", "-000001"},
		{-127, "-127", "-000127"},
		{-128, "-128", "-000128"},
		{-255, "-255", "-000255"},
		{-256, "-256", "-000256"},
		{-32767, "-32767", "-032767"},
		{-32768, "-32768", "-032768"}
	});
}

TEST_CASE("Decimal conversion from uint32_t", "[conversions]")
{
	testFromInt_t<uint32_t>::testDecConversions(
	{
		{0, "0", "00000000000"},
		{127, "127", "00000000127"},
		{128, "128", "00000000128"},
		{255, "255", "00000000255"},
		{256, "256", "00000000256"},
		{32767, "32767", "00000032767"},
		{32768, "32768", "00000032768"},
		{65535, "65535", "00000065535"},
		{65536, "65536", "00000065536"},
		{2147483647, "2147483647", "02147483647"},
		{2147483648, "2147483648", "02147483648"},
		{4294967295, "4294967295", "04294967295"}
	});
}

TEST_CASE("Decimal conversion from int32_t", "[conversions]")
{
	testFromInt_t<int32_t>::testDecConversions(
	{
		{0, "0", "000000000000"},
		{127, "127", "000000000127"},
		{128, "128", "000000000128"},
		{255, "255", "000000000255"},
		{256, "256", "000000000256"},
		{32767, "32767", "000000032767"},
		{32768, "32768", "000000032768"},
		{65535, "65535", "000000065535"},
		{65536, "65536", "000000065536"},
		{2147483647, "2147483647", "002147483647"},
		{-1, "-1", "-00000000001"},
		{-127, "-127", "-00000000127"},
		{-128, "-128", "-00000000128"},
		{-255, "-255", "-00000000255"},
		{-256, "-256", "-00000000256"},
		{-32767, "-32767", "-00000032767"},
		{-32768, "-32768", "-00000032768"},
		{-65535, "-65535", "-00000065535"},
		{-65536, "-65536", "-00000065536"},
		{-2147483647, "-2147483647", "-02147483647"},
#if __GNUC__ > 5
		{-2147483648, "-2147483648", "-02147483648"}
#endif
	});
}

TEST_CASE("Decimal conversion from uint64_t", "[conversions]")
{
	testFromInt_t<uint64_t>::testDecConversions(
	{
		{0, "0", "000000000000000000000"},
		{127, "127", "000000000000000000127"},
		{128, "128", "000000000000000000128"},
		{255, "255", "000000000000000000255"},
		{256, "256", "000000000000000000256"},
		{32767, "32767", "000000000000000032767"},
		{32768, "32768", "000000000000000032768"},
		{65535, "65535", "000000000000000065535"},
		{65536, "65536", "000000000000000065536"},
		{2147483647, "2147483647", "000000000002147483647"},
		{2147483648, "2147483648", "000000000002147483648"},
		{4294967295, "4294967295", "000000000004294967295"},
		{4294967296, "4294967296", "000000000004294967296"},
		{140737488355327, "140737488355327", "000000140737488355327"},
		{140737488355328, "140737488355328", "000000140737488355328"},
		{9223372036854775807, "9223372036854775807", "009223372036854775807"},
		{u64(9223372036854775808), "9223372036854775808", "009223372036854775808"},
		{u64(18446744073709551615), "18446744073709551615", "018446744073709551615"}
	});
}

TEST_CASE("Decimal conversion from int64_t", "[conversions]")
{
	testFromInt_t<int64_t>::testDecConversions(
	{
		{0, "0", "000000000000000000000"},
		{127, "127", "000000000000000000127"},
		{128, "128", "000000000000000000128"},
		{255, "255", "000000000000000000255"},
		{256, "256", "000000000000000000256"},
		{32767, "32767", "000000000000000032767"},
		{32768, "32768", "000000000000000032768"},
		{65535, "65535", "000000000000000065535"},
		{65536, "65536", "000000000000000065536"},
		{2147483647, "2147483647", "000000000002147483647"},
		{2147483648, "2147483648", "000000000002147483648"},
		{4294967295, "4294967295", "000000000004294967295"},
		{4294967296, "4294967296", "000000000004294967296"},
		{140737488355327, "140737488355327", "000000140737488355327"},
		{140737488355328, "140737488355328", "000000140737488355328"},
		{9223372036854775807, "9223372036854775807", "009223372036854775807"},
		{-1, "-1", "-00000000000000000001"},
		{-127, "-127", "-00000000000000000127"},
		{-128, "-128", "-00000000000000000128"},
		{-255, "-255", "-00000000000000000255"},
		{-256, "-256", "-00000000000000000256"},
		{-32767, "-32767", "-00000000000000032767"},
		{-32768, "-32768", "-00000000000000032768"},
		{-65535, "-65535", "-00000000000000065535"},
		{-65536, "-65536", "-00000000000000065536"},
		{-2147483647, "-2147483647", "-00000000002147483647"},
		{-2147483648, "-2147483648", "-00000000002147483648"},
		{-4294967295, "-4294967295", "-00000000004294967295"},
		{-4294967296, "-4294967296", "-00000000004294967296"},
		{-140737488355327, "-140737488355327", "-00000140737488355327"},
		{-140737488355328, "-140737488355328", "-00000140737488355328"},
		{-9223372036854775807, "-9223372036854775807", "-09223372036854775807"},
#if __GNUC__ > 5
		{i64(-9223372036854775807) - 1, "-9223372036854775808", "-09223372036854775808"}
#endif
	});
}

TEST_CASE("Hexadecimal conversion from uint8_t", "[conversions]")
{
	testFromInt_t<uint8_t>::testHexConversions(
	{
		{{0, "0", "000"}, true},
		{{127, "7F", "07F"}, true},
		{{128, "80", "080"}, true},
		{{255, "FF", "0FF"}, true},
		{{127, "7f", "07f"}, false},
		{{170, "aa", "0aa"}, false},
		{{255, "ff", "0ff"}, false}
	});
}

TEST_CASE("Hexadecimal conversion from uint16_t", "[conversions]")
{
	testFromInt_t<uint16_t>::testHexConversions(
	{
		{{0, "0", "00000"}, true},
		{{256, "100", "00100"}, true},
		{{32767, "7FFF", "07FFF"}, true},
		{{32768, "8000", "08000"}, true},
		{{65535, "FFFF", "0FFFF"}, true},
		{{32767, "7fff", "07fff"}, false},
		{{43690, "aaaa", "0aaaa"}, false},
		{{65535, "ffff", "0ffff"}, false}
	});
}

/*TEST_CASE("Hexadecimal conversion from uint32_t", "[conversions]")
{
	testFromInt_t<uint32_t>::testHexConversions(
	{
		{0, ""},
		{0, "00"},
		{65536, "00010000"},
		{2147483647, "7FFFFFFF"},
		{2147483648, "80000000"},
		{4294967295, "FFFFFFFF"}
	});
}

TEST_CASE("Hexadecimal conversion from uint64_t", "[conversions]")
{
	testFromInt_t<uint64_t>::testHexConversions(
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
}*/

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
