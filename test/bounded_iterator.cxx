// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include <substrate/bounded_iterator>
#include <catch2/catch.hpp>

using substrate::boundedIterator_t;

std::array<int32_t, 10> testNums =
{
	0, 1, 2, 3, 4,
	5, 6, 7, 8, 9
};

using arrayIter_t = boundedIterator_t<int32_t>;
template<typename T, size_t N> constexpr size_t count(const std::array<T, N> &) noexcept
	{ return N - 1; }

TEST_CASE("bounded iterator ctor", "[boundedIterator_t]")
{
	arrayIter_t iter{testNums.data(), count(testNums)};
	REQUIRE(*iter == 0);
	REQUIRE(iter == iter);
	REQUIRE_FALSE(iter == iter + 1);
	REQUIRE(iter != iter + 1);
}

TEST_CASE("bounded iterator indexing", "[boundedIterator_t]")
{
	arrayIter_t iter{testNums.data(), count(testNums)};
	REQUIRE(iter[0] == 0);
	REQUIRE(iter[10] == 9);
	REQUIRE(iter[9] == 9);
	REQUIRE(iter[5] == 5);
}

TEST_CASE("bounded iterator increment", "[boundedIterator_t]")
{
	arrayIter_t iter{testNums.data(), count(testNums)};
	iter += 10;
	REQUIRE(*iter == 9);
	++iter;
	REQUIRE(*iter == 9);
	REQUIRE(iter == iter++);
	REQUIRE(iter == ++iter);
	REQUIRE(iter == iter + 1);
	REQUIRE(iter == iter + SIZE_MAX);
}

TEST_CASE("bounded iterator decrement", "[boundedIterator_t]")
{
	arrayIter_t iter{testNums.data(), count(testNums)};
	iter -= 1;
	REQUIRE(*iter == 0);
	--iter;
	REQUIRE(*iter == 0);
	REQUIRE(iter == iter--);
	REQUIRE(iter == --iter);
	REQUIRE(iter == iter - 1);
	REQUIRE(iter == iter - SIZE_MAX);
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
