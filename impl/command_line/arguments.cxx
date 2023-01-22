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
		if (result.parseFrom(lexer, options))
			return std::nullopt;
		return result;
	}

	bool arguments_t::parseFrom(tokeniser_t &lexer, const options_t &options)
	{
		const auto &token{lexer.token()};
		while (token.valid())
		{
			if (!parseArgument(lexer, options))
			{
				const auto argument{token.value()};
				console.error("Found invalid token '"sv, argument, "' ("sv, typeToName(token.type()),
					") in arguments while parsing command line"sv);
				return false;
			}
		}
		return true;
	}

	bool arguments_t::parseArgument(tokeniser_t &lexer, const options_t &options)
	{
		const auto &token{lexer.token()};
		if (token.type() == tokenType_t::space)
			lexer.next();
		else if (token.type() != tokenType_t::arg)
			return false;
		const auto argument{token.value()};
		for (const auto &option : options)
		{
			const auto match
			{
				std::visit(match_t
				{
					[&](const option_t &option) -> std::optional<item_t>
					{
						if (option.matches(argument))
							return flag_t{};
						return std::nullopt;
					},
					[&](const optionSet_t &option) -> std::optional<item_t>
					{
						if (option.matches(argument))
							return choice_t{};
						return std::nullopt;
					}
				}, option)
			};
			// if (match)
			// 	return ast.add(*match);
		}
		lexer.next();
		return true;
	}
} // namespace substrate::commandLine

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
