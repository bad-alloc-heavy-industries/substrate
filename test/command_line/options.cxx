// SPDX-License-Identifier: BSD-3-Clause
#include <filesystem>
#include <substrate/conversions>
#include <substrate/command_line/options>
#include <catch2/catch.hpp>

using namespace std::literals::string_view_literals;
using std::filesystem::path;
using namespace substrate::commandLine;

enum class chipBus_t
{
	internal,
	external,
};

struct chip_t
{
	chipBus_t bus;
	uint8_t index;

	constexpr bool operator ==(const chip_t &other) const noexcept
		{ return bus == other.bus && index == other.index; }
};

TEST_CASE("building command line option descriptions", "[command_line::option_t]")
{
	constexpr static option_t helpOption{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv};
	REQUIRE(helpOption.matches("-h"sv));
	REQUIRE(helpOption.matches("--help"sv));
	REQUIRE(!helpOption.matches("--version"sv));
	REQUIRE(!helpOption.takesParameter());
	REQUIRE(!helpOption.isRepeatable());
	REQUIRE(!helpOption.valueOnly());
	REQUIRE(helpOption.metaName() == "help"sv);
	REQUIRE(helpOption.helpText() == "Display this help message and exit"sv);
	REQUIRE(!helpOption.parseValue(""sv).has_value());

	constexpr static option_t versionOption{"--version"sv, "Display the version information and exit"sv};
	REQUIRE(!versionOption.matches("-h"sv));
	REQUIRE(!versionOption.matches("--help"sv));
	REQUIRE(versionOption.matches("--version"sv));
	REQUIRE(!versionOption.takesParameter());
	REQUIRE(!versionOption.isRepeatable());
	REQUIRE(!versionOption.valueOnly());
	REQUIRE(versionOption.metaName() == "version"sv);
	REQUIRE(versionOption.helpText() == "Display the version information and exit"sv);
	REQUIRE(!helpOption.parseValue(""sv).has_value());

	constexpr static option_t valueOption{optionValue_t{"any"sv}, "Any value you like"sv};
	REQUIRE(valueOption.matches("funky"sv));
	REQUIRE(valueOption.helpText() == "Any value you like"sv);
	REQUIRE(valueOption.valueOnly());
	REQUIRE(valueOption.metaName() == "any"sv);
	REQUIRE(valueOption.parseValue(""sv).has_value());
	REQUIRE(std::any_cast<std::string_view>(*valueOption.parseValue("test"sv)) == "test"sv);
}

TEST_CASE("building command line option sets", "[command_line::optionSet_t]")
{
	constexpr static auto listOptions{options(option_t{"--all"sv, "List everything"sv})};

	constexpr static auto actions
	{
		optionAlternations
		({
			{
				"list"sv,
				"Lists things"sv,
				listOptions,
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
	constexpr static optionSet_t programOptions{"actions"sv, actions};

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

	REQUIRE(programOptions.metaName() == "actions"sv);
	checkMatch("list"sv, true);
	checkMatch("read"sv, true);
	checkMatch("write"sv, true);
	checkMatch("test"sv, false);

	REQUIRE_THROWS_AS(*actions[0].suboptions().end(), std::out_of_range);
}

template<typename T> void checkValue(const option_t &option, const std::string_view &inputValue, const T expectedValue)
{
	const auto result{option.parseValue(inputValue)};
	REQUIRE(result.has_value());
	const auto value{std::any_cast<T>(*result)};
	REQUIRE(value == expectedValue);
}

TEST_CASE("command line option value parsing", "[command_line::option_t::parseValue]")
{
	constexpr static auto unboundedUIntOption
	{
		option_t{""sv, ""sv}
			.takesParameter(optionValueType_t::unsignedInt)
	};

	REQUIRE(unboundedUIntOption.parseValue("-1"sv) == std::nullopt);
	REQUIRE(unboundedUIntOption.parseValue("a"sv) == std::nullopt);
	checkValue<uint64_t>(unboundedUIntOption, "0"sv, 0U);
	checkValue<uint64_t>(unboundedUIntOption, "18446744073709551615"sv, UINT64_C(18446744073709551615));

	constexpr static auto boundedUIntOption
	{
		option_t{""sv, ""sv}
			.takesParameter(optionValueType_t::unsignedInt)
			.valueRange(1U, 10U)
	};

	REQUIRE(boundedUIntOption.parseValue("a"sv) == std::nullopt);
	REQUIRE(boundedUIntOption.parseValue("0"sv) == std::nullopt);
	REQUIRE(boundedUIntOption.parseValue("11"sv) == std::nullopt);
	checkValue<uint64_t>(unboundedUIntOption, "1"sv, 1U);
	checkValue<uint64_t>(unboundedUIntOption, "10"sv, 10U);

	constexpr static auto unboundedIntOption
	{
		option_t{""sv, ""sv}
			.takesParameter(optionValueType_t::signedInt)
	};

	REQUIRE(unboundedIntOption.parseValue("a"sv) == std::nullopt);
	checkValue<int64_t>(unboundedIntOption, "-9223372036854775808"sv, INT64_C(-9223372036854775807) -1);
	checkValue<int64_t>(unboundedIntOption, "0"sv, 0);
	checkValue<int64_t>(unboundedIntOption, "9223372036854775807"sv, INT64_C(9223372036854775807));

	constexpr static auto boundedIntOption
	{
		option_t{""sv, ""sv}
			.takesParameter(optionValueType_t::signedInt)
			.valueRange(-10, 10)
	};
	REQUIRE(boundedIntOption.parseValue("a"sv) == std::nullopt);
	REQUIRE(boundedIntOption.parseValue("-11"sv) == std::nullopt);
	REQUIRE(boundedIntOption.parseValue("11"sv) == std::nullopt);
	checkValue<int64_t>(unboundedIntOption, "-10"sv, -10);
	checkValue<int64_t>(unboundedIntOption, "0"sv, 0);
	checkValue<int64_t>(unboundedIntOption, "10"sv, 10);

	constexpr static auto boolOption{option_t{""sv, ""sv}.takesParameter(optionValueType_t::boolean)};
	REQUIRE(boolOption.parseValue("0"sv) == std::nullopt);
	REQUIRE(boolOption.parseValue("a"sv) == std::nullopt);
	REQUIRE(boolOption.parseValue("1"sv) == std::nullopt);
	checkValue(boolOption, "true"sv, true);
	checkValue(boolOption, "false"sv, false);

	constexpr static auto pathOption{option_t{""sv, ""sv}.takesParameter(optionValueType_t::path)};
	checkValue(pathOption, "."sv, path{"."sv});
}

static inline std::optional<std::any> chipSelectionParser(const std::string_view &value) noexcept
{
	// Check if we have enough characters to form a valid chip selector
	if (value.length() < 5)
		return std::nullopt;
	// If we do, check that the start of the string is one of "int" or "ext"
	const auto bus{value.substr(0, 3)};
	if (bus != "int"sv && bus != "ext"sv)
		return std::nullopt;
	// Check that the 4th character is a ':'
	if (value[3] != ':')
		return std::nullopt;
	// Now convert the final part as an unsigned integer
	substrate::toInt_t<uint64_t> index{value.substr(4).data()};
	if (!index.isDec())
		return std::nullopt;
	const auto number{index.fromDec()};
	// And check that it's in the range [0,256)
	if (number > 255U)
		return std::nullopt;
	// Construct and return the chip selection state
	return chip_t{bus[0] == 'i' ? chipBus_t::internal : chipBus_t::external, static_cast<uint8_t>(number)};
}

TEST_CASE("command line option user-defined value parsing", "[command_line::option_t::parseValue]")
{
	constexpr static auto userDefinedOption
	{
		option_t
		{
			"--chip"sv,
			"Specifies what Flash chip on which bus you want to target.\n"
			"The chip specification works as follows:\n"
			"'bus' can be one of 'int' or 'ext' representing the internal (on-chip)\n"
			"and external (8-pin Flash connector) SPI busses.\n"
			"N is a number from 0 to 255 which specifies a detected Flash chip as given by the\n"
			"listDevices operation"sv,
		}.takesParameter(optionValueType_t::userDefined, chipSelectionParser)
	};

	REQUIRE(userDefinedOption.parseValue(""sv) == std::nullopt);
	REQUIRE(userDefinedOption.parseValue("out:0"sv) == std::nullopt);
	REQUIRE(userDefinedOption.parseValue("int#0"sv) == std::nullopt);
	REQUIRE(userDefinedOption.parseValue("ext:a"sv) == std::nullopt);
	REQUIRE(userDefinedOption.parseValue("ext:256"sv) == std::nullopt);
	checkValue<chip_t>(userDefinedOption, "int:0"sv, {chipBus_t::internal, 0U});
	checkValue<chip_t>(userDefinedOption, "ext:20"sv, {chipBus_t::external, 20U});
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
