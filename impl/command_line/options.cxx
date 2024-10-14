// SPDX-License-Identifier: BSD-3-Clause
#include <filesystem>
#include <locale>
#include <numeric>
#include <substrate/command_line/options>
#include <substrate/command_line/arguments>
#include <substrate/console>
#include <substrate/conversions>
#include <substrate/index_sequence>
#include <substrate/utility>

using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;
using substrate::console;

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
			[](const optionValue_t &) { return true; }
		}, _option);
	}

	std::optional<std::any> option_t::parseValue(const std::string_view &value) const noexcept
	{
		if (!takesParameter())
			return std::nullopt;
		switch (_valueType)
		{
			case optionValueType_t::signedInt:
				// If the option takes a signed integer, dispatch to the signed int parser
				return parseSignedValue(value);
			case optionValueType_t::unsignedInt:
				// If the option takes a unsigned integer, dispatch to the unsigned int parser
				return parseUnsignedValue(value);
			case optionValueType_t::boolean:
				// If the option takes a boolean, dispatch to the bool parser
				return parseBoolValue(value);
			case optionValueType_t::string:
				// If the option takes a string, do nothing and return the string
				return value;
			case optionValueType_t::path:
				// If the option takes a path, dispatch to the path parser
				return parsePathValue(value);
			case optionValueType_t::userDefined:
				// If the value type is used defined, dispatch to the user's custom value parser
				return _valueParserFn(value);
		}
		// If we reach here, something's gone wrong
		return std::nullopt;
	}

	// NOTLINENEXTLINE(readability-convert-member-functions-to-static)
	std::optional<std::any> option_t::parseSignedValue(const std::string_view &value) const noexcept
	{
		toInt_t<int64_t> number{value.data()};
		// Check if the value to parse is actually numeric
		if (!number.isDec())
			return std::nullopt;
		// Now do the conversion
		const auto result{number.fromDec()};
		// Check if we have a range restriction
		if (_valueAllowedRange.valueless_by_exception() ||
			!std::holds_alternative<optionSignedInt_t>(_valueAllowedRange))
			// Return the result of conversion unchecked if we do not
			return result;
		const auto &range{std::get<optionSignedInt_t>(_valueAllowedRange)};
		// Range check the result of conversion
		if (result < range.min || result > range.max)
			return std::nullopt;
		// Now we know the value is in the proper range, return it
		return result;
	}

	// NOTLINENEXTLINE(readability-convert-member-functions-to-static)
	std::optional<std::any> option_t::parseUnsignedValue(const std::string_view &value) const noexcept
	{
		toInt_t<uint64_t> number{value.data()};
		// Check if the value to parse is actually numeric
		if (!number.isDec())
			return std::nullopt;
		// Now do the conversion
		const auto result{number.fromDec()};
		// Check if we have a range restriction
		if (_valueAllowedRange.valueless_by_exception() ||
			!std::holds_alternative<optionUnsignedInt_t>(_valueAllowedRange))
			// Return the result of conversion unchecked if we do not
			return result;
		const auto &range{std::get<optionUnsignedInt_t>(_valueAllowedRange)};
		// Range check the result of conversion
		if (result < range.min || result > range.max)
			return std::nullopt;
		// Now we know the value is in the proper range, return it
		return result;
	}

	// NOTLINENEXTLINE(readability-convert-member-functions-to-static)
	std::optional<std::any> option_t::parseBoolValue(const std::string_view &value) noexcept
	{
		// If the value matches the strings "true" or "false", convert to the boolean equivilent
		if (value == "true"sv)
			return true;
		if (value == "false"sv)
			return false;
		// If not then parsing fails
		return std::nullopt;
	}

	// NOTLINENEXTLINE(readability-convert-member-functions-to-static)
	std::optional<std::any> option_t::parsePathValue(const std::string_view &value) noexcept
	{
		// There's unfortunately not much of any validation can be done on a path, so..
		// This function just converts the string_view ith a path object and returns it
		std::filesystem::path path{value};
		return path;
	}

	bool optionAlternation_t::matches(const std::string_view &argument) const noexcept
		{ return argument == _value; }

	// NOTLINENEXTLINE(readability-convert-member-functions-to-static)
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

	// This calculates how much padding is needed for these options to get a consistent padding
	[[nodiscard]] size_t optionSet_t::displayPadding() const noexcept
	{
		return std::accumulate(begin(), end(), size_t{},
			[](const size_t padding, const optionAlternation_t &alternation) noexcept
				{ return std::max(padding, alternation.displayLength()); }
		);
	}

	// NOTLINENEXTLINE(readability-convert-member-functions-to-static)
	void optionSet_t::displayHelp(const size_t padding) const noexcept
	{
		console.writeln();
		const auto &metaName{_metaName.substr(1)};
		console.writeln(std::toupper(_metaName[0], std::locale::classic()), metaName, "s:"sv);
		for (const auto &alternation : _alternations)
		{
			console.writeln('\t', alternation.displayName(), nullptr);
			for ([[maybe_unused]] const auto _ : substrate::indexSequence_t{alternation.displayLength(), padding})
				console.writeln(' ', nullptr);
			console.writeln(' ', alternation.helpText());
		}
	}

	bool optionSet_t::displayHelp(const arguments_t &args) const noexcept
	{
		const auto arg{args[_metaName]};
		// We found a match so we now need to find out which one matched
		if (arg)
		{
			// Extract the choice made and loop through the possible choice alternations
			const auto &choice{std::get<choice_t>(*arg)};
			for (const auto &option : _alternations)
			{
				// If the alternation matches the one actually selected, display it
				if (option.matches(choice.value()))
					option.suboptions().displayHelp(choice.arguments(), _metaName);
			}
		}
		// Return if we found a match
		return arg;
	}

	// Implementation of the innards of optionSet_t as otherwise we get compile errors
	const optionAlternation_t *optionSet_t::begin() const noexcept
		{ return _alternations.begin(); }
	const optionAlternation_t *optionSet_t::end() const noexcept
		{ return _alternations.end(); }
	bool optionSet_t::operator <(const optionSet_t &other) const noexcept
		{ return _alternations.data() < other._alternations.data(); }

	[[nodiscard]] static inline auto unprefix(const std::string_view &flag) noexcept
	{
		auto name = flag;
		// Remove hyphens from the front until either the string's empty or we've got no more
		while (!name.empty() && name[0] == '-')
			name = name.substr(1);
		// Make sure we return something that's not ""sv if possible
		return name.empty() ? flag : name;
	}

	[[nodiscard]] std::string_view option_t::metaName() const noexcept
	{
		if (_option.valueless_by_exception())
			return ""sv;
		return std::visit(match_t
		{
			[](const std::string_view &option) { return unprefix(option); },
			[](const optionFlagPair_t &option) { return unprefix(option._longFlag); },
			[](const optionValue_t &option) { return option.metaName(); },
		}, _option);
	}

	[[nodiscard]] inline std::string_view option_t::typeToValue() const noexcept
	{
		switch (_valueType)
		{
			case optionValueType_t::signedInt:
				return "INT"sv;
			case optionValueType_t::unsignedInt:
				return "UINT"sv;
			case optionValueType_t::boolean:
				return "BOOL"sv;
			case optionValueType_t::string:
				return "STRING"sv;
			case optionValueType_t::path:
				return "PATH"sv;
			case optionValueType_t::userDefined:
				return "VAL"sv;
		}
		substrate::unreachable();
	}

	// NOTLINENEXTLINE(readability-convert-member-functions-to-static)
	[[nodiscard]] std::string option_t::displayName() const noexcept
	{
		if (_option.valueless_by_exception())
			return ""s;
		const auto typeValue
		{
			[&]() -> std::string
			{
				if (!takesParameter())
					return {};
				return " "s + std::string{typeToValue()};
			}()
		};
		return std::visit(match_t
		{
			[&](const std::string_view &option) { return std::string{option} + typeValue; },
			[&](const optionFlagPair_t &option)
				{ return std::string{option._shortFlag} + ", "s + std::string{option._longFlag} + typeValue; },
			[&](const optionValue_t &option) { return std::string{option.metaName()} + (isRepeatable() ? "..."s : ""s); }
		}, _option);
	}

	// NOTLINENEXTLINE(readability-convert-member-functions-to-static)
	[[nodiscard]] size_t option_t::displayLength() const noexcept
	{
		const auto value_length
		{
			[&]() -> size_t
			{
				if (!takesParameter())
					return 0U;
				return typeToValue().length() + 1U;
			}()
		};
		return std::visit(match_t
		{
			[&](const std::string_view &option) { return option.length() + value_length; },
			[&](const optionFlagPair_t &option)
				// Add the lengths of the two flags together, and the extra ", " that is inserted by displayName()
				{ return option._shortFlag.length() + option._longFlag.length() + 2U + value_length; },
			[&](const optionValue_t &option) { return option.metaName().length() + (isRepeatable() ? 3U : 0U); },
		}, _option);
	}

	void option_t::displayHelp(const size_t padding) const noexcept
	{
		console.writeln('\t', displayName(), nullptr);
		for ([[maybe_unused]] const auto _ : substrate::indexSequence_t{displayLength(), padding})
			console.writeln(' ', nullptr);
		console.writeln(' ', _help);
	}

	namespace internal
	{
		// This calculates how much padding is needed for these options to get a consistent padding
		[[nodiscard]] size_t optionsHolder_t::displayPadding() const noexcept
		{
			return std::accumulate(begin(), end(), size_t{},
				[](const size_t padding, const optionsItem_t &option) noexcept
				{
					return std::max(padding, std::visit(match_t
						{
							[](const option_t &value) { return value.displayLength(); },
							[](const optionSet_t &value) { return value.displayPadding(); },
						}, option)
					);
				}
			);
		}

		// NOTLINENEXTLINE(readability-convert-member-functions-to-static)
		void optionsHolder_t::displayHelp(const arguments_t &args, const std::string_view &optionsTitle) const noexcept
		{
			// First check to see if we should actually be displaying some alternation's help
			for (const auto &option : *this)
			{
				const auto handled
				{
					std::visit(match_t
					{
						[](const option_t &) { return false; },
						[&](const optionSet_t &optionSet) { return optionSet.displayHelp(args); },
					}, option)
				};
				// If we have indeed managed to handle things, finish early
				if (handled)
					return;
			}

			// No optionSet_t's to traverse, so now figure out how much padding is needed to make everything neat
			const auto padding{displayPadding()};
			std::vector<optionSet_t> optionSets{};

			// Display the title block for the non-alternation options
			if (optionsTitle.empty())
				console.writeln("Options:"sv);
			else
				console.writeln(std::toupper(optionsTitle[0], std::locale::classic()), optionsTitle.substr(1),
					"s options:"sv);

			// Now display the non-alternation options and collect option sets
			for (const auto &option : *this)
			{
				std::visit(match_t
				{
					[=](const option_t &value) { value.displayHelp(padding); },
					[&](const optionSet_t &value) { optionSets.push_back(value); },
				}, option);
			}

			// Now go through each option alternation set and display those
			for (const auto &optionSet : optionSets)
				optionSet.displayHelp(padding);
		}

		// NOTLINENEXTLINE(readability-convert-member-functions-to-static)
		void optionsRoot_t::displayHelp(const arguments_t &args) const noexcept
		{
			// Display the help header first
			if (!_helpHeader.empty())
			{
				console.writeln(_helpHeader);
				console.writeln();
			}

			if (!_usage.empty())
			{
				console.writeln("Usage:"sv);
				console.writeln('\t', _usage);
				console.writeln();
			}

			// Now display the appropriate options help
			_options.displayHelp(args);

			// Finish up by displaying the help footer
			if (!_helpFooter.empty())
			{
				console.writeln();
				console.writeln(_helpFooter);
			}
		}
	} // namespace internal
} // namespace substrate::commandLine

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
