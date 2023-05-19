// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include <substrate/zip_container>
#include <catch2/catch.hpp>

using substrate::zipContainer_t;

static constexpr std::array<uint8_t, 10> testNumsU8 =
{{
	0, 1, 2, 3, 4,
	5, 6, 7, 8, 9
}};

static constexpr std::array<int16_t, 10> testNumsI16 =
{{
	0, 1, 2, 3, 0xFF,
	5, 6, 7, 8, 9
}};

static constexpr std::array<int32_t, 10> testNumsI32 =
{{
	0, 1, 2, 3, 4,
	5, 6, 7, 8, 9
}};

using containerIter_t = zipContainer_t<decltype(testNumsU8), decltype(testNumsI16), decltype(testNumsI32)>;

TEST_CASE("zip container ctor", "[zipContainer_t]")
{
	containerIter_t container{testNumsU8, testNumsI16, testNumsI32};
	(void)container;
}

TEST_CASE("zip container indexing", "[zipContainer_t]")
{
	containerIter_t container{testNumsU8, testNumsI16, testNumsI32};
	auto iter = container.begin();
	REQUIRE(std::get<0>(iter[0]) == 0);
	REQUIRE(std::get<1>(iter[9]) == 9);
	REQUIRE(std::get<0>(iter[4]) == 4);
	REQUIRE(std::get<1>(iter[4]) == 0xFF);
	REQUIRE(std::get<2>(iter[4]) == 4);
}

TEST_CASE("zip container increment", "[zipContainer_t]")
{
	containerIter_t container{testNumsU8, testNumsI16, testNumsI32};
	auto iter = container.begin();
	iter += 9;
	{
		auto tuple = *iter;
		REQUIRE(std::get<0>(tuple) == 9);
		REQUIRE(std::get<1>(tuple) == 9);
		REQUIRE(std::get<2>(tuple) == 9);
	}
	++iter;
	REQUIRE(iter == container.end());
	REQUIRE(iter == iter++);
	REQUIRE(iter == ++iter);
	REQUIRE(iter == iter + 1);
	REQUIRE(iter == iter + std::numeric_limits<decltype(iter)::difference_type>::max());
}

TEST_CASE("zip container decrement", "[zipContainer_t]")
{
	containerIter_t container{testNumsU8, testNumsI16, testNumsI32};
	auto iter = container.begin();
	iter -= 1;
	{
		auto tuple = *iter;
		REQUIRE(std::get<0>(tuple) == 0);
		REQUIRE(std::get<1>(tuple) == 0);
		REQUIRE(std::get<2>(tuple) == 0);
	}
	--iter;
	{
		auto tuple = *iter;
		REQUIRE(std::get<0>(tuple) == 0);
		REQUIRE(std::get<1>(tuple) == 0);
		REQUIRE(std::get<2>(tuple) == 0);
	}
	REQUIRE(iter == iter--);
	REQUIRE(iter == --iter);
	REQUIRE(iter == iter - 1);
	REQUIRE(iter == iter - std::numeric_limits<decltype(iter)::difference_type>::max());
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
