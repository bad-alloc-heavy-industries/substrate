// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/command_line/options>
#include <substrate/command_line/arguments>
#include <substrate/utility>
#include <catch2/catch.hpp>

using namespace std::literals::string_view_literals;
using namespace substrate::commandLine;

TEST_CASE("parseArguments invalid invocations", "[command_line::parseArguments]")
{
	constexpr std::array<const char *, 1U> dummyArgs{{nullptr}};
	constexpr static options_t dummyOptions{};

	REQUIRE(parseArguments(0U, dummyArgs.data(), dummyOptions) == std::nullopt);
	REQUIRE(parseArguments(dummyArgs.size(), nullptr, dummyOptions) == std::nullopt);
	REQUIRE(parseArguments(static_cast<size_t>(-1), dummyArgs.data(), dummyOptions) == std::nullopt);
}

TEST_CASE("parse command line argument choice", "[command_line::parseArguments]")
{
	constexpr static auto actions
	{
		optionAlternations
		({
			{
				"choiceA"sv,
				"First action choice"sv,
			},
			{
				"choiceB"sv,
				"Second action choice"sv,
			},
		})
	};

	constexpr static auto programOptions{options(optionSet_t{actions})};

	// Check if parsing the first alternation works
	constexpr static auto argsChoiceA
	{
		substrate::make_array<const char *>
		({
			"program",
			"choiceA",
			nullptr,
		})
	};
	const auto resultA{parseArguments(argsChoiceA.size(), argsChoiceA.data(), programOptions)};
	REQUIRE(resultA != std::nullopt);

	// Check if parsing the second works
	constexpr static auto argsChoiceB
	{
		substrate::make_array<const char *>
		({
			"program",
			"choiceB",
			nullptr,
		})
	};
	const auto resultB{parseArguments(argsChoiceB.size(), argsChoiceB.data(), programOptions)};
	REQUIRE(resultB != std::nullopt);

	// Check that parsing a non-existant choice value fails
	constexpr static auto argsChoiceC
	{
		substrate::make_array<const char *>
		({
			"program",
			"choiceC",
			nullptr,
		})
	};
	const auto resultC{parseArguments(argsChoiceC.size(), argsChoiceC.data(), programOptions)};
	// REQUIRE(resultC == std::nullopt);
}
