// SPDX-License-Identifier: BSD-3-Clause
#include <filesystem>
#include <substrate/command_line/options>
#include <substrate/command_line/arguments>
#include <substrate/console>
#include <substrate/utility>
#include <catch2/catch.hpp>

using namespace std::literals::string_view_literals;
using std::filesystem::path;
using substrate::console;
using namespace substrate::commandLine;

TEST_CASE("parseArguments invalid invocations", "[command_line::parseArguments]")
{
	console = {stdout, stderr};
	constexpr std::array<const char *, 1U> dummyArgs{{nullptr}};
	constexpr static options_t dummyOptions{};

	// Check the handling of the various combinations of invalid argc/argv inputs
	REQUIRE(parseArguments(0U, dummyArgs.data(), dummyOptions) == std::nullopt);
	REQUIRE(parseArguments(dummyArgs.size(), nullptr, dummyOptions) == std::nullopt);
	REQUIRE(parseArguments(static_cast<size_t>(-1), dummyArgs.data(), dummyOptions) == std::nullopt);

	// Check that the options validator works correctly in detecting an options_t with multiple option_t's
	// that are set up as optionValue_t{}'s
	constexpr static auto programOptionsA
	{
		options
		(
			option_t{optionValue_t{"meow"sv}, "meow"sv},
			option_t{optionValue_t{"gao"sv}, "gao"sv}
		)
	};
	REQUIRE(parseArguments(dummyArgs.size(), dummyArgs.data(), programOptionsA) == std::nullopt);

	// Check that the options validator works correctly in detecting the same as the above, but embedded
	// inside an optionSet_t option alternation
	constexpr static auto actionOptions
	{
		options
		(
			option_t{optionValue_t{"meow"sv}, "meow"sv},
			option_t{optionValue_t{"gao"sv}, "gao"sv}
		)
	};

	constexpr static auto actions
	{
		optionAlternations
		({
			{
				"action"sv,
				"help text"sv,
				actionOptions,
			}
		})
	};

	constexpr static auto programOptionsB{options(optionSet_t{actions})};
	REQUIRE(parseArguments(dummyArgs.size(), dummyArgs.data(), programOptionsB) == std::nullopt);
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
	REQUIRE(argsA.begin() != argsA.end());
	REQUIRE(&*argsA.begin() == &argsA[0]);

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

TEST_CASE("parse command line argument simple flag", "[command_line::parseArguments]")
{
	console = {stdout, stderr};
	constexpr static auto programOptions
	{
		options
		(
			option_t{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv},
			option_t{"--version"sv, "Display the version information for flashprog and exit"sv}
		)
	};

	// Check to see if parsing a simple flag works right
	constexpr static auto argsHelpA
	{
		substrate::make_array<const char *>
		({
			"program",
			"--help",
			nullptr,
		})
	};
	const auto resultHelpA{parseArguments(argsHelpA.size(), argsHelpA.data(), programOptions)};
	const auto &argsA{checkResult(resultHelpA, 1U)};
	REQUIRE(std::holds_alternative<flag_t>(*argsA.begin()));
	const auto &helpA{std::get<flag_t>(*argsA.begin())};
	REQUIRE(helpA.name() == "--help"sv);
	REQUIRE(!helpA.value().has_value());

	constexpr static auto argsHelpB
	{
		substrate::make_array<const char *>
		({
			"program",
			"-h",
			nullptr,
		})
	};
	const auto resultHelpB{parseArguments(argsHelpB.size(), argsHelpB.data(), programOptions)};
	const auto &argsB{checkResult(resultHelpB, 1U)};
	REQUIRE(std::holds_alternative<flag_t>(*argsB.begin()));
	const auto &helpB{std::get<flag_t>(*argsB.begin())};
	REQUIRE(helpB.name() == "-h"sv);
	REQUIRE(!helpB.value().has_value());

	// Check that parsing correctly steps over non-matching flags
	constexpr static auto argsVersion
	{
		substrate::make_array<const char *>
		({
			"program",
			"--version",
			nullptr,
		})
	};
	const auto resultVersion{parseArguments(argsVersion.size(), argsVersion.data(), programOptions)};
	const auto &argsVers{checkResult(resultVersion, 1U)};
	REQUIRE(std::holds_alternative<flag_t>(*argsVers.begin()));
	const auto &version{std::get<flag_t>(*argsVers.begin())};
	REQUIRE(version.name() == "--version"sv);
	REQUIRE(!version.value().has_value());

	// Check that parsing an invalid option fails
	constexpr static auto argsV
	{
		substrate::make_array<const char *>
		({
			"program",
			"-v",
			nullptr,
		})
	};
	const auto resultV{parseArguments(argsV.size(), argsV.data(), programOptions)};
	REQUIRE(resultV == std::nullopt);
}

TEST_CASE("parse command line argument flags", "[command_line::parseArguments]")
{
	console = {stdout, stderr};
	constexpr static auto programOptions
	{
		options
		(
			option_t{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv},
			option_t{optionFlagPair_t{"-v"sv, "--verbosity"sv}, "Set the log output verbosity"sv}
				.takesParameter(optionValueType_t::unsignedInt).valueRange(0U, 63U),
			option_t{optionValue_t{"workingDir"sv}, "Working directory"sv}.valueType(optionValueType_t::path),
			option_t{"--option", "Operation-specific options"sv}.takesParameter().repeatable()
		)
	};

	// Check that valued options work as intended
	constexpr static auto argsValues
	{
		substrate::make_array<const char *>
		({
			"program",
			"-v", "10",
			"--option=A",
			"/some/path",
			"--option", "B",
			nullptr,
		})
	};
	const auto resultValues{parseArguments(argsValues.size(), argsValues.data(), programOptions)};
	const auto &args{checkResult(resultValues, 4U)};

	REQUIRE(std::holds_alternative<flag_t>(args[0]));
	const auto &verbosity{std::get<flag_t>(args[0])};
	REQUIRE(verbosity.name() == "-v"sv);
	REQUIRE(verbosity.value().has_value());
	REQUIRE(std::any_cast<uint64_t>(verbosity.value()) == 10U);

	REQUIRE(std::holds_alternative<flag_t>(args[1]));
	const auto &optionA{std::get<flag_t>(args[1])};
	REQUIRE(optionA.name() == "--option"sv);
	REQUIRE(optionA.value().has_value());
	REQUIRE(std::any_cast<std::string_view>(optionA.value()) == "A"sv);

	REQUIRE(std::holds_alternative<flag_t>(args[3]));
	const auto &optionB{std::get<flag_t>(args[3])};
	REQUIRE(optionB.name() == "--option"sv);
	REQUIRE(optionB.value().has_value());
	REQUIRE(std::any_cast<std::string_view>(optionB.value()) == "B"sv);

	REQUIRE(std::holds_alternative<flag_t>(args[2]));
	const auto &workingDir{std::get<flag_t>(args[2])};
	// XXX: optionValue_t{} needs to be given the ability to hold a meta name for the value so
	// these can be looked up sensibly
	REQUIRE(workingDir.name() == "workingDir"sv);
	REQUIRE(workingDir.value().has_value());
	REQUIRE(std::any_cast<path>(workingDir.value()) == path{"/some/path"sv});

	// Check that parsing a truncated valued option fails as intended
	constexpr static auto argsTruncated
	{
		substrate::make_array<const char *>
		({
			"program",
			"--verbosity",
			nullptr,
		})
	};
	const auto resultTruncated{parseArguments(argsTruncated.size(), argsTruncated.data(), programOptions)};
	REQUIRE(resultTruncated == std::nullopt);

	// Check that parsing a valued option with a bad value fails as intended
	constexpr static auto argsBad
	{
		substrate::make_array<const char *>
		({
			"program",
			"-v", "a",
			nullptr,
		})
	};
	const auto resultBad{parseArguments(argsBad.size(), argsBad.data(), programOptions)};
	REQUIRE(resultBad == std::nullopt);
}

TEST_CASE("parse bad command line argument flags", "[command_line::parseArguments]")
{
	console = {stdout, stderr};
	constexpr static auto valueOptions
		{options(option_t{optionValue_t{"value"sv}, "value"sv}.valueType(optionValueType_t::unsignedInt))};
	constexpr static auto choiceAOptions{options(option_t{"--test"sv, "Run action in test mode"sv})};

	constexpr static auto actions
	{
		optionAlternations
		({
			{
				"choiceA"sv,
				"First action choice"sv,
				choiceAOptions,
			},
			{
				"choiceB"sv,
				"Second action choice"sv,
			},
		})
	};

	constexpr static auto programOptions
	{
		options
		(
			option_t{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv},
			option_t{optionFlagPair_t{"-v"sv, "--verbosity"sv}, "Set the log output verbosity"sv}
				.takesParameter(optionValueType_t::unsignedInt).valueRange(0U, 63U),
			option_t{"--option", "Operation-specific options"sv}.takesParameter().repeatable(),
			optionSet_t{actions}
		)
	};

	constexpr static auto argsInvalidA
	{
		substrate::make_array<const char *>
		({
			"program",
			"--test=value",
			nullptr,
		})
	};
	const auto resultInvalidA{parseArguments(argsInvalidA.size(), argsInvalidA.data(), programOptions)};
	REQUIRE(resultInvalidA == std::nullopt);

	constexpr static auto argsInvalidB
	{
		substrate::make_array<const char *>
		({
			"program",
			"--test=",
			"--option=A",
			nullptr,
		})
	};
	const auto resultInvalidB{parseArguments(argsInvalidB.size(), argsInvalidB.data(), programOptions)};
	REQUIRE(resultInvalidB == std::nullopt);

	constexpr static auto argsInvalidC
	{
		substrate::make_array<const char *>
		({
			"program",
			"choiceA",
			"--param",
			nullptr,
		})
	};
	const auto resultInvalidC{parseArguments(argsInvalidC.size(), argsInvalidC.data(), programOptions)};
	REQUIRE(resultInvalidC == std::nullopt);

	constexpr static auto argsInvalidD
	{
		substrate::make_array<const char *>
		({
			"program",
			"value",
			nullptr,
		})
	};
	const auto resultInvalidD{parseArguments(argsInvalidD.size(), argsInvalidD.data(), valueOptions)};
	REQUIRE(resultInvalidD == std::nullopt);
}

TEST_CASE("parse duplicated command line arguments", "[command_line::parseArguments]")
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

	constexpr static auto programOptions
	{
		options
		(
			option_t{optionFlagPair_t{"-h"sv, "--help"sv}, "Display this help message and exit"sv},
			option_t{optionFlagPair_t{"-v"sv, "--verbosity"sv}, "Set the log output verbosity"sv}
				.takesParameter(optionValueType_t::unsignedInt).valueRange(0U, 63U),
			option_t{optionValue_t{"workingDir"sv}, "Working directory"sv}.valueType(optionValueType_t::path),
			optionSet_t{actions}
		)
	};

	constexpr static auto argsInvalidA
	{
		substrate::make_array<const char *>
		({
			"program",
			"-h",
			"--help",
			nullptr,
		})
	};
	const auto resultInvalidA{parseArguments(argsInvalidA.size(), argsInvalidA.data(), programOptions)};
	REQUIRE(resultInvalidA == std::nullopt);

	constexpr static auto argsInvalidB
	{
		substrate::make_array<const char *>
		({
			"program",
			"-v", "10",
			"choiceA",
			"choiceB",
			nullptr,
		})
	};
	const auto resultInvalidB{parseArguments(argsInvalidB.size(), argsInvalidB.data(), programOptions)};
	REQUIRE(resultInvalidB == std::nullopt);

	constexpr static auto argsInvalidC
	{
		substrate::make_array<const char *>
		({
			"program",
			"-v", "10",
			"choiceA",
			"-v", "63",
			nullptr,
		})
	};
	const auto resultInvalidC{parseArguments(argsInvalidC.size(), argsInvalidC.data(), programOptions)};
	REQUIRE(resultInvalidC == std::nullopt);
}
