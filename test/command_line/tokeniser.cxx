// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include <substrate/command_line/tokeniser>
#include <substrate/utility>
#include <catch2/catch.hpp>

using namespace std::literals::string_view_literals;
using substrate::commandLine::internal::tokeniser_t;
using substrate::commandLine::internal::tokenType_t;
using substrate::commandLine::internal::typeToName;

constexpr static auto commandLine
{
	substrate::make_array<const char *>(
	{
		"arg",
		"--option=value",
		"--other-option",
		"second value",
	})
};

TEST_CASE("token type to name conversion", "[command_line::internal::typeToName]")
{
	REQUIRE(typeToName(tokenType_t::unknown) == "unknown"sv);
	REQUIRE(typeToName(tokenType_t::arg) == "argument"sv);
	REQUIRE(typeToName(tokenType_t::space) == "implicit space"sv);
	REQUIRE(typeToName(tokenType_t::equals) == "equals sign"sv);
	REQUIRE(typeToName(static_cast<tokenType_t>(255U)) == std::string_view{});
}

static void checkNextIsSpace(tokeniser_t &tokeniser)
{
	const auto token{tokeniser.next()};
	REQUIRE(token.valid());
	REQUIRE(token.type() == tokenType_t::space);
}

static void checkNextIsEquals(tokeniser_t &tokeniser)
{
	const auto token{tokeniser.next()};
	REQUIRE(token.valid());
	REQUIRE(token.type() == tokenType_t::equals);
	REQUIRE(token.value() == "="sv);
}

TEST_CASE("command line tokenisation", "[command_line::internal::tokeniser_t]")
{
	tokeniser_t tokeniser{commandLine.size(), commandLine.data()};
	const auto &token{tokeniser.token()};

	REQUIRE(token.valid());
	REQUIRE(token.type() == tokenType_t::arg);
	REQUIRE(token.value() == "arg"sv);

	checkNextIsSpace(tokeniser);
	tokeniser.next();

	REQUIRE(token.valid());
	REQUIRE(token.type() == tokenType_t::arg);
	REQUIRE(token.value() == "--option"sv);

	checkNextIsEquals(tokeniser);
	tokeniser.next();

	REQUIRE(token.valid());
	REQUIRE(token.type() == tokenType_t::arg);
	REQUIRE(token.value() == "value"sv);

	checkNextIsSpace(tokeniser);
	tokeniser.next();

	REQUIRE(token.valid());
	REQUIRE(token.type() == tokenType_t::arg);
	REQUIRE(token.value() == "--other-option"sv);

	checkNextIsSpace(tokeniser);
	tokeniser.next();

	REQUIRE(token.valid());
	REQUIRE(token.type() == tokenType_t::arg);
	REQUIRE(token.value() == "second value"sv);

	tokeniser.next();
	REQUIRE(!token.valid());
	REQUIRE(token.type() == tokenType_t::unknown);
}
