// SPDX-License-Identifier: BSD-3-Clause
#include <cstring>
#include <substrate/command_line/tokeniser>

using namespace std::literals::string_view_literals;

namespace substrate::commandLine::internal
{
	void tokenizer_t::nextArg() noexcept
	{
		if (_currentArg == _count)
		{
			_arg = nullptr;
			_offset = nullptr;
			_length = 0;
		}
		else
		{
			_arg = _args[_currentArg++];
			_offset = _arg;
			_length = strlen(_arg);
		}
	}

	[[nodiscard]] static inline auto isEquals(const char chr) noexcept { return chr == '='; }

	void tokenizer_t::readToken() noexcept
	{
		if (!_arg)
			_token = {};
		else if (_offset == _arg + _length)
		{
			nextArg();
			if (!_arg)
				_token = {};
			else
				_token = {tokenType_t::space};
		}
		else if (isEquals(*_offset))
		{
			++_offset;
			_token = {tokenType_t::equals, {"=", 1}};
		}
		else
		{
			const auto *const begin{_offset};
			while (_offset < _arg + _length)
			{
				if (isEquals(*_offset))
					break;
				++_offset;
			}
			_token = {tokenType_t::arg, {begin, size_t(_offset - begin)}};
		}
	}

	std::string_view typeToName(const tokenType_t type) noexcept
	{
		switch (type)
		{
		case tokenType_t::unknown:
			return "unknown"sv;
		case tokenType_t::arg:
			return "argument"sv;
		case tokenType_t::space:
			return "implicit space"sv;
		case tokenType_t::equals:
			return "equals sign"sv;
		}
		return {};
	}
} // namespace substrate::commandLine::internal

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
