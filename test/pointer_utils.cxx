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

TEST_CASE("managed release check through convertible values", "[managedTupleWithValues_t]")
{
	bool released = false;

	{
		using releaser_t = void(std::array<int, 1>*, float*);

		std::function<releaser_t> releaser = [&](std::array<int, 1> *x, float *a) -> void
		{
			REQUIRE(x->at(0) == 1);
			REQUIRE(*a == 2.0F);
			released = true;
		};

		substrate::managedTupleWithValues_t<releaser_t, std::array<int, 1>&, float&> sample{releaser};

		const float a = 2.0F;
		const std::array<int, 1> b = {1};

		REQUIRE(sample.size() == 2);
		sample.template get<0>() = b;
		sample.template get<1>() = a;
	}

	REQUIRE(released);
}

#endif

static int releaseInt(int *value)
{
	REQUIRE(value != nullptr);
	REQUIRE(*value == 1);
	delete value; // NOLINT(*-owning-memory)
	return 1;
};

static void releaseFloat(float *value)
{
	REQUIRE(value != nullptr);
	REQUIRE(*value == 2.0F);
	delete value; // NOLINT(*-owning-memory)
};

TEST_CASE("naughty (non-void-returning) deleter", "[naughtyPtr_t]")
{
	using naughtyPtr = substrate::naughtyPtr_t<int, int, releaseInt>;

	naughtyPtr sample{new int(1)};

	REQUIRE(*sample == 1);
	sample.reset();
	REQUIRE(sample.use_count() == 0);
}

TEST_CASE("nice (void-returning) deleter", "[nicePtr_t]")
{
	using nicePtr = substrate::nicePtr_t<decltype(releaseFloat), &releaseFloat>;

	nicePtr sample{new float(2.0F)};

	REQUIRE(*sample == 2.0F);
	sample.reset();
	REQUIRE(sample.use_count() == 0);
}
