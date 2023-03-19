// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/command_line/options>
#include <catch2/catch.hpp>

using namespace std::literals::string_view_literals;
using namespace substrate::commandLine;

TEST_CASE("building command line option descriptions", "[command_line::option_t]")
{
	const option_t helpOption{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv};
	REQUIRE(helpOption.matches("-h"sv));
	REQUIRE(helpOption.matches("--help"sv));
	REQUIRE(!helpOption.matches("--version"sv));
	REQUIRE(!helpOption.takesParameter());
	REQUIRE(!helpOption.isRepeatable());
	REQUIRE(helpOption.helpText() == "Display this help message and exit"sv);
	REQUIRE(!helpOption.parseValue(""sv).has_value());

	const option_t versionOption{"--version"sv, "Display the version information and exit"sv};
	REQUIRE(!versionOption.matches("-h"sv));
	REQUIRE(!versionOption.matches("--help"sv));
	REQUIRE(versionOption.matches("--version"sv));
	REQUIRE(!versionOption.takesParameter());
	REQUIRE(!versionOption.isRepeatable());
	REQUIRE(versionOption.helpText() == "Display the version information and exit"sv);
	REQUIRE(!helpOption.parseValue(""sv).has_value());
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
