// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/string>
#include <catch.hpp>

using substrate::stringDup;

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
