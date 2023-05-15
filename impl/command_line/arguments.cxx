// SPDX-License-Identifier: BSD-3-Clause
#include <limits>
#include <algorithm>
#include <substrate/console>
#include <substrate/command_line/arguments>
#include <substrate/command_line/tokeniser>

using namespace std::literals::string_view_literals;

namespace substrate::commandLine
{
	using namespace internal;

	template<typename... Ts> struct match_t : Ts... { using Ts::operator()...; };
	template<typename... Ts> match_t(Ts...) -> match_t<Ts...>;

	bool operator <(const optionsItem_t &lhs, const optionsItem_t &rhs) noexcept
	{
		// First, if the alternative held is numerically < then sort on that
		if (lhs.index() < rhs.index())
			return true;
		// Check if the alternatives are instead numerically >
		if (lhs.index() > rhs.index())
			return false;

		// Next, both are the same index.. convert to the held alternative and do < on those
		if (std::holds_alternative<option_t>(lhs))
			return std::get<option_t>(lhs) < std::get<option_t>(rhs);
		if (std::holds_alternative<optionSet_t>(lhs))
			return std::get<optionSet_t>(lhs) < std::get<optionSet_t>(rhs);
		return false;
	}

	static bool validateOptions(const options_t &options) noexcept;

	// This implements a recursive descent parser that efficiently matches the current token from argv against
	// the set of allowed arguments at the current parsing level, and returns an AST of the results for
	// later easier exploration by the caller
	std::optional<arguments_t> parseArguments(size_t argCount, const char *const *argList, const options_t &options)
	{
		constexpr static auto intBits{std::numeric_limits<int>::digits - 1U};
		// Check if we even got any arguments or if argv was negative, or if the options input is malformed in any way
		if (!argCount || (argCount & (1U << intBits)) || !argList || !validateOptions(options))
			return std::nullopt;
		// The first argument is the name of the program, so skip that at least and start at the second.
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		tokeniser_t lexer{argCount - 1U, argList + 1};
		arguments_t result{};
		// Try to parse all available arguments against the options tree for the program
		if (!result.parseFrom(lexer, options))
			return std::nullopt;
		return result;
	}

	static bool validateOptions(const optionSet_t &optionSet) noexcept
	{
		// Check that every alternation in the set is valid
		return std::all_of(optionSet.begin(), optionSet.end(),
			[](const optionAlternation_t &alternation)
				{ return validateOptions(alternation.suboptions()); }
		);
	}

	static bool validateOptions(const options_t &options) noexcept
	{
		size_t valueOptions{};
		for (const auto &option : options)
		{
			if (!std::visit(match_t
				{
					[&](const option_t &value)
					{
						valueOptions += value.valueOnly() ? 1U : 0U;
						return true;
					},
					[](const optionSet_t &value) { return validateOptions(value); }
				}, option)
			)
				return false;
		}
		if (valueOptions > 1U)
			console.error("Command line options define multiple value-only options, only at most one is allowed, got "sv,
				valueOptions);
		return valueOptions < 2U;
	}

	bool arguments_t::parseFrom(tokeniser_t &lexer, const options_t &options)
	{
		const auto &token{lexer.token()};
		optionsVisited_t optionsVisited{};
		while (token.valid())
		{
			const auto result{parseArgument(lexer, options, optionsVisited)};
			// If the result is a nullopt, we're unwinding an inner failure
			if (!result)
				return false;
			// Else if the result is false, we just experienced failure that needs reporting
			if (!*result)
			{
				const auto argument{token.value()};
				console.error("Found invalid token '"sv, argument, "' ("sv, typeToName(token.type()),
					") in arguments while parsing command line"sv);
				return false;
			}
		}
		return true;
	}

	using optionMatch_t = std::variant<flag_t, choice_t, std::monostate>;
	static std::optional<optionMatch_t> matchOption(tokeniser_t &lexer, const option_t &option,
		const std::string_view &argument) noexcept;
	static std::optional<optionMatch_t> matchOptionSet(tokeniser_t &lexer, const optionSet_t &option,
		const std::string_view &argument);
	template<typename set_t> static bool checkMatchValid(const optionsItem_t &option, set_t &optionsVisited);
	template<typename set_t> static std::optional<bool> handleResult(arguments_t &arguments, const optionsItem_t &option,
		set_t &optionsVisited, const std::string_view &argument, const optionMatch_t &match) noexcept;
	static void handleUnrecognised(tokeniser_t &lexer, const std::string_view &argument);

	std::optional<bool> arguments_t::parseArgument(tokeniser_t &lexer, const options_t &options,
		optionsVisited_t &optionsVisited)
	{
		// Start by checking we're in a suitable state
		const auto &token{lexer.token()};
		if (token.type() == tokenType_t::space)
			lexer.next();
		// If we're not, fast-fail
		else if (token.type() != tokenType_t::arg)
			return false;
		// Next, grab the argument string and start trying to match it to an option
		const auto argument{token.value()};
		// Initialise look-aside for optionValue_t{} options
		std::optional<option_t> valueOption{};
		for (const auto &option : options)
		{
			// Check if this option is an option_t that is valueOnly() (optionValue_t{})
			if (std::holds_alternative<option_t>(option))
			{
				const auto &value{std::get<option_t>(option)};
				if (value.valueOnly())
				{
					valueOption = value;
					continue;
				}
			}

			// Otherwise, process the option normally
			const auto match
			{
				// Dispatch based on the option type
				std::visit(match_t
				{
					[&](const option_t &value) { return matchOption(lexer, value, argument); },
					[&](const optionSet_t &value) { return matchOptionSet(lexer, value, argument); },
				}, option)
			};

			// If we got a valid match, use the result
			if (match)
				return handleResult(*this, option, optionsVisited, argument, *match);
		}
		// If there's an optionValue_t{} and we got no match so far, try matching on it
		if (valueOption)
		{
			const auto match{matchOption(lexer, *valueOption, argument)};
			// If we got a valid match, use the result
			if (match)
				return handleResult(*this, *valueOption, optionsVisited, argument, *match);
		}
		// After trying to match, if we got nothing, pass it through the unrecognised argument machinary
		handleUnrecognised(lexer, argument);
		// We should now be one token past the end of the unrecognised argument,
		// but turn this into a failure return all the same
		return false;
	}

	bool arguments_t::add(item_t argument) noexcept try
	{
		// Try to push back the new parsed item
		_arguments.push_back(std::move(argument));
		return true;
	}
	// Catch any falure and turn that into a clean error result
	catch (std::exception &)
		{ return false; }

	static std::optional<optionMatch_t> matchOption(tokeniser_t &lexer, const option_t &option,
		const std::string_view &argument) noexcept
	{
		// Check if we're parsing a "simple" option
		if (!option.matches(argument))
			return std::nullopt;
		// If the option matches, try parsing out and validating the value portion if there is one
		if (!option.takesParameter())
		{
			lexer.next();
			return flag_t{option.metaName()};
		}
		const auto &token{lexer.token()};
		// Consume tokens to get to the value token if the option is not value-only
		if (!option.valueOnly())
		{
			lexer.next();
			// Check that we have a parameter component following
			if (token.type() != tokenType_t::equals && token.type() != tokenType_t::space)
				// If the operation fails, use monostate to signal match-but-fail.
				return std::monostate{};
			lexer.next();
		}
		// Try parsing that parameter component as a value for the option
		auto value{option.parseValue(token.value())};
		lexer.next();
		if (!value)
			// If the operation fails, use monostate to signal match-but-fail.
			return std::monostate{};
		// If it succeeds, grab the meta name to use as the flag name and return the parsed value
		return flag_t{option.metaName(), std::move(*value)};
	}

	static std::optional<optionMatch_t> matchOptionSet(tokeniser_t &lexer, const optionSet_t &option,
		const std::string_view &argument)
	{
		// Check if we're parsing an alternation from a set
		const auto match{option.matches(argument)};
		if (match)
		{
			// Check which alternation matched, recurse and parse all further options from the
			// alternation's perspective
			const auto &alternation{match->get()};
			lexer.next();
			arguments_t subarguments{};
			const auto &suboptions{alternation.suboptions()};
			if (!suboptions.empty() && !subarguments.parseFrom(lexer, suboptions))
				// If the operation fails, use monostate to signal match-but-fail.
				return std::monostate{};
			return choice_t{option.metaName(), argument, std::move(subarguments)};
		}
		return std::nullopt;
	}

	template<typename set_t> static bool checkMatchValid(const optionsItem_t &option, set_t &optionsVisited)
	{
		// Look for the option in the set
		const auto &count{optionsVisited.find(option)};
		if (count != optionsVisited.end())
		{
			// if this is an optionSet_t, those are never allowed to repeat.
			if (!std::holds_alternative<option_t>(option))
				return false;
			// Otherwise, check if the argument is allowed to repeat
			const auto &value{std::get<option_t>(option)};
			return value.isRepeatable();
		}
		// This is our first time seeing this one, so allow it and add it to the map
		optionsVisited.insert(option);
		return true;
	}

	template<typename set_t> static std::optional<bool> handleResult(arguments_t &arguments, const optionsItem_t &option,
		set_t &optionsVisited, const std::string_view &argument, const optionMatch_t &match) noexcept
	{
		// First check if the match is allowed by the option
		if (!checkMatchValid(option, optionsVisited))
		{
			console.error("Command line argument '"sv, argument,
				"' already seen on command line, and repetitions are not allowed"sv);
			return false;
		}
		// If the check succeeds, run with it
		return std::visit(match_t
		{
			// We got a match and parsing it succeeded?
			[&]([[maybe_unused]] const auto &result) -> std::optional<bool> { return arguments.add(result); },
			// Match but inner parsing failed
			[](std::monostate) -> std::optional<bool> { return std::nullopt; },
		}, match);
	}

	static void handleUnrecognised(tokeniser_t &lexer, const std::string_view &argument)
	{
		const auto &token{lexer.next()};
		// If the argument is followed by an '=', grab both parts of it for display
		if (token.type() == tokenType_t::equals)
		{
			lexer.next();
			// If there's nothing after the '=', display what we've got
			if (token.type() == tokenType_t::space)
				console.error("Unrecognised command line argument '"sv, argument, "='"sv);
			else
			{
				const auto value{token.value()};
				console.error("Unrecognised command line argument '"sv, argument, '=', value, "'"sv);
				lexer.next();
			}
		}
		else
			console.error("Unrecognised command line argument '"sv, argument, "'"sv);
	}

	// Implementation of the innards of arguments_t as otherwise we get compile errors
	// NOLINTNEXTLINE(modernize-use-equals-default)
	arguments_t::arguments_t() noexcept : _arguments{} { }
	size_t arguments_t::count() const noexcept
		{ return _arguments.size(); }
	arguments_t::iterator_t arguments_t::begin() const noexcept
		{ return _arguments.begin(); }
	arguments_t::iterator_t arguments_t::end() const noexcept
		{ return _arguments.end(); }
	const item_t &arguments_t::operator [](size_t index) const noexcept
		{ return _arguments[index]; }

	arguments_t::iterator_t arguments_t::find(const std::string_view &option) const noexcept
	{
		return std::find_if(_arguments.begin(), _arguments.end(),
			[&](const item_t &argument)
			{
				return std::visit(match_t
				{
					[&](const flag_t &value) { return value.name() == option; },
					[&](const choice_t &value) { return value.name() == option; },
				}, argument);
			}
		);
	}

	const item_t *arguments_t::operator [](const std::string_view &option) const noexcept
	{
		const auto &argument{find(option)};
		if (argument == end())
			return nullptr;
		return &*argument;
	}
} // namespace substrate::commandLine

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
