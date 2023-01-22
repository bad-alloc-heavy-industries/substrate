// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/command_line/options>

namespace substrate::commandLine
{
	template<typename... Ts> struct match_t : Ts... { using Ts::operator()...; };
	template<typename... Ts> match_t(Ts...) -> match_t<Ts...>;

	bool option_t::matches(const std::string_view &argument) const noexcept
	{
		return std::visit(match_t
		{
			[&](const std::string_view &option) { return argument == option; },
			[&](const optionFlagPair_t &options)
				{ return argument == options._shortFlag || argument == options._longFlag; },
			[](const std::monostate &) { return true; }
		}, _option);
	}

	bool optionAlternation_t::matches(const std::string_view &argument) const noexcept
		{ return argument == _value; }

	std::optional<std::reference_wrapper<const optionAlternation_t>>
		optionSet_t::matches(const std::string_view &argument) const noexcept
	{
		for (const auto &alternation : _alternations)
		{
			if (alternation.matches(argument))
				return std::ref(alternation);
		}
		return std::nullopt;
	}
} // namespace substrate::commandLine

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
