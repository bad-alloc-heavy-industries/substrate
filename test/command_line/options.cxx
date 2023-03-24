// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/command_line/options>
#include <catch2/catch.hpp>

using namespace std::literals::string_view_literals;
using namespace substrate::commandLine;

TEST_CASE("building command line option descriptions", "[command_line::option_t]")
{
	constexpr static option_t helpOption{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv};
	REQUIRE(helpOption.matches("-h"sv));
	REQUIRE(helpOption.matches("--help"sv));
	REQUIRE(!helpOption.matches("--version"sv));
	REQUIRE(!helpOption.takesParameter());
	REQUIRE(!helpOption.isRepeatable());
	REQUIRE(!helpOption.valueOnly());
	REQUIRE(helpOption.helpText() == "Display this help message and exit"sv);
	REQUIRE(!helpOption.parseValue(""sv).has_value());

	constexpr static option_t versionOption{"--version"sv, "Display the version information and exit"sv};
	REQUIRE(!versionOption.matches("-h"sv));
	REQUIRE(!versionOption.matches("--help"sv));
	REQUIRE(versionOption.matches("--version"sv));
	REQUIRE(!versionOption.takesParameter());
	REQUIRE(!versionOption.isRepeatable());
	REQUIRE(!versionOption.valueOnly());
	REQUIRE(versionOption.helpText() == "Display the version information and exit"sv);
	REQUIRE(!helpOption.parseValue(""sv).has_value());

	constexpr static option_t valueOption{optionValue_t{}, "Any value you like"sv};
	REQUIRE(valueOption.matches("funky"sv));
	REQUIRE(valueOption.helpText() == "Any value you like"sv);
	REQUIRE(valueOption.valueOnly());
	REQUIRE(valueOption.parseValue(""sv).has_value());
	REQUIRE(std::any_cast<std::string_view>(*valueOption.parseValue("test"sv)) == "test"sv);
}

TEST_CASE("building command line option sets", "[command_line::optionSet_t]")
{
	static const auto listOptions
	{
		options
		({
			{option_t{"--all"sv, "List everything"sv}},
		})
	};

	static const auto actions
	{
		optionAlternations
		({
			{
				"list"sv,
				"Lists things"sv,
				{listOptions},
			},
			{
				"read"sv,
				"Reads things"sv,
			},
			{
				"write"sv,
				"Writes things"sv,
			},
		})
	};
	static const optionSet_t programOptions{actions};

	const auto checkMatch
	{
		[&](const std::string_view &value, const bool expectingMatch)
		{
			INFO("Matching against '" << value << "'");
			const auto match{programOptions.matches(value)};
			if (expectingMatch)
			{
				REQUIRE(match.has_value());
				if (value == "list"sv)
				{
					const auto &alternation{match->get()};
					const auto &suboptions{alternation.suboptions()};
					REQUIRE(!suboptions.empty());
					const auto &begin{*suboptions.begin()};
					REQUIRE(std::holds_alternative<option_t>(begin));
					const auto &option{std::get<option_t>(begin)};
					REQUIRE(option.matches("--all"sv));
				}
				else
					REQUIRE(match->get().suboptions().empty());
			}
			else
				REQUIRE(!match.has_value());
		}
	};

	checkMatch("list"sv, true);
	checkMatch("read"sv, true);
	checkMatch("write"sv, true);
	checkMatch("test"sv, false);
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
