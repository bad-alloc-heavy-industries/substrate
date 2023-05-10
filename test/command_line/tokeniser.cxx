// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/command_line/tokeniser>
#include <catch2/catch.hpp>

using namespace std::literals::string_view_literals;
using substrate::commandLine::internal::tokeniser_t;
using substrate::commandLine::internal::tokenType_t;
using substrate::commandLine::internal::typeToName;

TEST_CASE("token type to name conversion", "[command_line::internal::typeToName")
{
	REQUIRE(typeToName(tokenType_t::unknown) == "unknown"sv);
	REQUIRE(typeToName(tokenType_t::arg) == "argument"sv);
	REQUIRE(typeToName(tokenType_t::space) == "implicit space"sv);
	REQUIRE(typeToName(tokenType_t::equals) == "equals sign"sv);
	REQUIRE(typeToName(static_cast<tokenType_t>(255U)) == std::string_view{});
}
