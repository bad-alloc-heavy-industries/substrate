// SPDX-License-Identifier: BSD-3-Clause
#include <limits>
#include <substrate/console>
#include <substrate/command_line/arguments>
#include <substrate/command_line/tokeniser>

using namespace std::literals::string_view_literals;

namespace substrate::commandLine
{
	using namespace internal;

	template<typename... Ts> struct match_t : Ts... { using Ts::operator()...; };
	template<typename... Ts> match_t(Ts...) -> match_t<Ts...>;

	// This implemnts a recursive descent parser that efficiently matches the current token from argv against
	// the set of allowed arguments at the current parsing level, and returns an AST of the results for
	// later easier exploration by the caller
	std::optional<arguments_t> parseArguments(size_t argCount, const char *const *argList, const options_t &options)
	{
		constexpr static auto intBits{std::numeric_limits<int>::digits - 1U};
		// Check if we even got any arguments or if argv was negative
		if (!argCount || (argCount & (1U << intBits)) || !argList)
			return std::nullopt;
		// The first argument is the name of the program, so skip that at least and start at the second.
		tokeniser_t lexer{argCount - 1U, argList + 1};
		arguments_t result{};
		// Try to parse all available arguments against the options tree for the program
		if (!result.parseFrom(lexer, options))
			return std::nullopt;
		return result;
	}

	bool arguments_t::parseFrom(tokeniser_t &lexer, const options_t &options)
	{
		const auto &token{lexer.token()};
		while (token.valid())
		{
			const auto result{parseArgument(lexer, options)};
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

	std::optional<bool> arguments_t::parseArgument(tokeniser_t &lexer, const options_t &options)
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
		for (const auto &option : options)
		{
			const auto match
			{
				// Dispatch based on the option type
				std::visit(match_t
				{
					[&](const option_t &value) { return matchOption(lexer, value, argument); },
					[&](const optionSet_t &value) { return matchOptionSet(lexer, value, argument); },
				}, option)
			};

			// If we got a valid match, us the result
			if (match)
			{
				return std::visit(match_t
				{
					// We got a match and parsing it succeeded?
					[this]([[maybe_unused]] const auto &result) -> std::optional<bool>
						{ return true; /*add(result);*/ },
					// Match but inner parsing failed
					[](std::monostate) -> std::optional<bool>
						{ return std::nullopt; },
				}, *match);
			}
		}
		// XXX: Need to handle the no-match situation.
		lexer.next();
		return true;
	}

	static std::optional<optionMatch_t> matchOption(tokeniser_t &lexer, const option_t &option,
		const std::string_view &argument) noexcept
	{
		const auto &token{lexer.token()};
		// Check if we're parsing a "simple" option
		if (!option.matches(argument))
			return std::nullopt;
		// If the option matches, try parsing out and validating the value portion if there is one
		if (!option.takesParameter())
			return flag_t{argument};
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
		if (!value)
			// If the operation fails, use monostate to signal match-but-fail.
			return std::monostate{};
		return flag_t{argument, std::move(*value)};
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
			return choice_t{};
		}
		return std::nullopt;
	}
} // namespace substrate::commandLine

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
