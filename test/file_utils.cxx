// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include <substrate/file_utils>
#include <substrate/indexed_iterator>
#include <catch2/catch_test_macros.hpp>

using substrate::fd_t;
using substrate::indexedIterator_t;
using substrate::read_lines;

TEST_CASE("lines reader", "[boundedIterator_t]")
{
	const std::array<std::string, 6> lines
	{
		{
			"## Enforcement Responsibilities 1",
			"",
			"Community leaders are responsible for clarifying and enforcing our standards of",
			"acceptable behavior and will take appropriate and fair corrective action in",
			"response to any behavior that they deem inappropriate, threatening, offensive,",
			"or harmful."
		}
	};

	{
		fd_t text{"info.txt", O_WRONLY | O_CREAT | O_EXCL | O_TEXT, substrate::normalMode};
		REQUIRE(text.valid());
		for (const auto &i : lines)
		{
			REQUIRE(text.write(i));
			REQUIRE(text.write('\n'));
		}
	}

	{
		fd_t text{"info.txt", O_RDONLY | O_TEXT};
		REQUIRE(text.valid());
		// Memory mapping here will cause the conversion to be skipped
		const auto input {read_lines(text)};
		REQUIRE(lines.size() == input.size());
		for (const auto &line : indexedIterator_t<decltype(lines)>{lines})
		{
			const auto &contents {line.second};
			const auto &input_line {input[line.first]};
			REQUIRE(input_line.find_first_of('\n', input_line.size() - 1) == std::string::npos);
			REQUIRE(input_line.find_first_of('\r', input_line.size() - 1) == std::string::npos);
			REQUIRE(input_line == contents);
		}
	}

	{
		const auto result{unlink("info.txt")};
		if (result != 0)
			REQUIRE(errno == 0);
		else
			SUCCEED();
	}
}

TEST_CASE("lines reader with std::u16string", "[boundedIterator_t]")
{
	const std::array<std::u16string, 6> lines
	{{
		u"## Enforcement Responsibilities 2",
		u"",
		u"Community leaders are responsible for clarifying and enforcing our standards of",
		u"acceptable behavior and will take appropriate and fair corrective action in",
		u"response to any behavior that they deem inappropriate, threatening, offensive,",
		u"or harmful."
	}};

	{
		fd_t text{"info.txt", O_WRONLY | O_CREAT | O_EXCL | O_BINARY, substrate::normalMode};
		REQUIRE(text.valid());
		for (const auto &line : lines)
		{
			REQUIRE(text.write(line.data(), line.size() * sizeof(std::u16string::value_type)));
			REQUIRE(text.write(u'\n'));
		}
	}

	{
		fd_t text{"info.txt", O_RDONLY | O_BINARY};
		REQUIRE(text.valid());
		const auto input{substrate::read_lines<std::u16string>(text)};
		REQUIRE(lines.size() == input.size());
		for (const auto &line : indexedIterator_t<decltype(lines)>{lines})
		{
			const auto &contents {line.second};
			const auto &input_line {input[line.first]};
			REQUIRE(input_line.find_first_of(u'\n', input_line.size() - 1) == std::u16string::npos);
			REQUIRE(input_line.find_first_of(u'\r', input_line.size() - 1) == std::u16string::npos);
			REQUIRE(input_line == contents);
		}
	}

	{
		const auto result{unlink("info.txt")};
		if (result != 0)
			REQUIRE(errno == 0);
		else
			SUCCEED();
	}
}

TEST_CASE()
{
	unlink("info.txt");
	SUCCEED();
}
