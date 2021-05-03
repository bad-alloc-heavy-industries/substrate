// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <array>
#include <substrate/indexed_iterator>
#include <catch.hpp>

using substrate::indexedIterator_t;

constexpr static std::array<char, 6> container
{{
	'a', 'b', 'c', 'd', 'e', 'f'
}};

TEST_CASE("enumerated iteration", "[indexedIterator_t]")
{
	indexedIterator_t<decltype(container)> iter{container};
	const auto begin{iter.begin()};
	const auto end{iter.end()};
	REQUIRE(begin.index() == 0);
	REQUIRE(*begin.item() == 'a');
	REQUIRE(end.index() == 6);

	size_t value{0};
	for (const auto item : iter)
	{
		REQUIRE(item.first == value);
		REQUIRE(*item.second == container[value]);
		++value;
	}
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
