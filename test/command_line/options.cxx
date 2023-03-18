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
	REQUIRE(!helpOption.takesParameter());
	REQUIRE(!helpOption.isRepeatable());
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
