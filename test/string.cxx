// SPDX-License-Identifier: BSD-3-Clause
#include <cstring>
#include <substrate/string>
#include <catch2/catch.hpp>

using substrate::stringDup;
using substrate::stringsLength;
using substrate::stringConcat;

static const std::string string = "This is only a test";

TEST_CASE("string raw dup", "[stringDup]")
{
	REQUIRE_FALSE(stringDup(nullptr));
	auto result = stringDup(string.data());
	REQUIRE(result);
	REQUIRE(memcmp(result.get(), string.data(), string.size()) == 0);
}

TEST_CASE("string unique_ptr dup", "[stringDup]")
{
	std::unique_ptr<char []> stringPtr{};
	REQUIRE_FALSE(stringDup(stringPtr));
	stringPtr = stringDup(string.data());
	auto result = stringDup(stringPtr);
	REQUIRE(result);
	REQUIRE(memcmp(result.get(), string.data(), string.size()) == 0);
}

TEST_CASE("strings length", "[stringsLength]")
{
	REQUIRE(stringsLength("") == 0);
	REQUIRE(stringsLength("T") == 1);
	REQUIRE(stringsLength("The ", "quick") == 9);
	REQUIRE(stringsLength("The ", "quick ", "fox") == 13);
}

TEST_CASE("string concat", "[stringConcat]")
{
	const auto a = stringConcat("");
	REQUIRE(a);
	REQUIRE(memcmp(a.get(), "", 1) == 0);

	const auto b = stringConcat("The");
	REQUIRE(b);
	REQUIRE(memcmp(b.get(), "The", 4) == 0);

	const auto c = stringConcat("Hello", "world");
	REQUIRE(c);
	REQUIRE(memcmp(c.get(), "Helloworld", 11) == 0);

	const auto d = stringConcat("Goodbye", " cruel ", "world");
	REQUIRE(d);
	REQUIRE(memcmp(d.get(), "Goodbye cruel world", 20) == 0);
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
