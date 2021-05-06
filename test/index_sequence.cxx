// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <substrate/index_sequence>
#include <catch2/catch.hpp>

using substrate::indexIterator_t;
using substrate::indexSequence_t;

TEST_CASE("simple range iteration", "[indexSequence_t]")
{
	indexSequence_t range{10};
	REQUIRE(*range.begin() == 0);
	REQUIRE(*range.end() == 10);
	size_t value{0};
	for (const auto idx : range)
	{
		REQUIRE(idx == value);
		++value;
	}
}

TEST_CASE("advanced range iteration", "[indexSequence_t]")
{
	indexSequence_t range{4, 12};
	REQUIRE(*range.begin() == 4);
	REQUIRE(*range.end() == 12);
	size_t value{4};
	for (const auto idx : range)
	{
		REQUIRE(idx == value);
		++value;
	}
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
