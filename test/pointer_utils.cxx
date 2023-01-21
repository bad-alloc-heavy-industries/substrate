// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include <substrate/pointer_utils>
#include <catch2/catch.hpp>

#if defined SUBSTRATE_SUPPORTS_MANAGED_TUPLE

TEST_CASE("managed release check through references", "[managedTuple_t]")
{
	bool released = false;

	{
		using releaser_t = void(std::array<int, 1>&, std::pair<int, float>&, int&);

		std::function<releaser_t> releaser = [&](std::array<int, 1> &x, std::pair<int, float> &y, int &a) -> void
		{
			REQUIRE(x[0] == 1);
			REQUIRE(y.first == 5);
			REQUIRE(y.second == 1.0F);
			REQUIRE(a == 2);
			released = true;
		};

		substrate::managedTuple_t<releaser_t> sample{releaser};

		REQUIRE(sample.size() == 3);
		sample.template get<0>() = {1};
		sample.template get<1>() = {5, 1.0F};
		sample.template get<2>() = {2};
	}

	REQUIRE(released);
}

TEST_CASE("managed release check through pointers", "[managedTuple_t]")
{
	bool released = false;

	{
		using releaser_t = void(std::array<int, 1>*, std::pair<int, float>*, int*);

		std::function<releaser_t> releaser = [&](std::array<int, 1> *x, std::pair<int, float> *y, int *a) -> void
		{
			REQUIRE(x->at(0) == 1);
			REQUIRE(y->first == 5);
			REQUIRE(y->second == 1.0F);
			REQUIRE(*a == 2);
			released = true;
		};

		substrate::managedTuple_t<releaser_t> sample{releaser};

		REQUIRE(sample.size() == 3);
		sample.template get<0>() = {1};
		sample.template get<1>() = {5, 1.0F};
		sample.template get<2>() = {2};
	}

	REQUIRE(released);
}

#endif
