// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/command_line/options>
#include <substrate/conversions>

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

	std::optional<std::any> option_t::parseSignedValue(const std::string_view &value) const noexcept
	{
		toInt_t<int64_t> number{value.data()};
		// Check if the value to parse is actually numeric
		if (!number.isDec())
			return std::nullopt;
		// Now do the conversion
		const auto result{number.fromDec()};
		// Check if we have a range restriction
		if (!std::holds_alternative<optionSignedInt_t>(_valueAllowedRange))
			// Return the result of conversion unchecked if we do not
			return result;
		const auto &range{std::get<optionSignedInt_t>(_valueAllowedRange)};
		// Range check the result of conversion
		if (result < range.min || result > range.max)
			return std::nullopt;
		// Now we know the value is in the proper range, return it
		return result;
	}

	std::optional<std::any> option_t::parseUnsignedValue(const std::string_view &value) const noexcept
	{
		toInt_t<uint64_t> number{value.data()};
		// Check if the value to parse is actually numeric
		if (!number.isDec())
			return std::nullopt;
		// Now do the conversion
		const auto result{number.fromDec()};
		// Check if we have a range restriction
		if (!std::holds_alternative<optionUnsignedInt_t>(_valueAllowedRange))
			// Return the result of conversion unchecked if we do not
			return result;
		const auto &range{std::get<optionUnsignedInt_t>(_valueAllowedRange)};
		// Range check the result of conversion
		if (result < range.min || result > range.max)
			return std::nullopt;
		// Now we know the value is in the proper range, return it
		return result;
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
