// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/command_line/options>
#include <substrate/command_line/arguments>
#include <substrate/console>
#include <substrate/utility>
#include <catch2/catch.hpp>

using namespace std::literals::string_view_literals;
using substrate::console;
using namespace substrate::commandLine;

TEST_CASE("parseArguments invalid invocations", "[command_line::parseArguments]")
{
	constexpr std::array<const char *, 1U> dummyArgs{{nullptr}};
	constexpr static options_t dummyOptions{};

	REQUIRE(parseArguments(0U, dummyArgs.data(), dummyOptions) == std::nullopt);
	REQUIRE(parseArguments(dummyArgs.size(), nullptr, dummyOptions) == std::nullopt);
	REQUIRE(parseArguments(static_cast<size_t>(-1), dummyArgs.data(), dummyOptions) == std::nullopt);
}

static const auto &checkResult(const std::optional<arguments_t> &result, const size_t expectedCount)
{
	REQUIRE(result.has_value());
	const auto &args{*result};
	REQUIRE(args.count() == expectedCount);
	return args;
}

TEST_CASE("parse command line argument choice", "[command_line::parseArguments]")
{
	console = {stdout, stderr};
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
	const auto &argsA{checkResult(resultA, 1U)};
	REQUIRE(std::holds_alternative<choice_t>(*argsA.begin()));
	const auto &choiceA{std::get<choice_t>(*argsA.begin())};
	REQUIRE(choiceA.value() == "choiceA"sv);
	REQUIRE(choiceA.arguments().count() == 0U);

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
	const auto &argsB{checkResult(resultB, 1U)};
	REQUIRE(std::holds_alternative<choice_t>(*argsB.begin()));
	const auto &choiceB{std::get<choice_t>(*argsB.begin())};
	REQUIRE(choiceB.value() == "choiceB"sv);
	REQUIRE(choiceB.arguments().count() == 0U);

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
	REQUIRE(resultC == std::nullopt);
}
