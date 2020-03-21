#include <cstring>
#include <substrate/fixed_vector>
#include <catch.hpp>

using substrate::fixedVector_t;
using substrate::vectorStateException_t;

template<typename T, typename E> void testThrowsExcept(T &vec, const char *const errorText)
{
	try
	{
		int value = vec[2];
		(void)value;
		FAIL("fixedVector_t<> failed to throw exception when expected");
	}
	catch (const E &except)
		{ REQUIRE(memcmp(except.what(), errorText, strlen(errorText)) == 0); }
}

template<typename T> struct typeOfVector;
template<typename T> struct typeOfVector<fixedVector_t<T>> { using type = T; };
template<typename T> struct typeOfVector<const fixedVector_t<T>> { using type = const T; };

template<typename T, typename value_t = typename typeOfVector<T>::type>
	value_t &index(T &vec, const size_t index) { return vec[index]; }

template<typename T> void testIterEqual(T &vec)
	{ REQUIRE(vec.begin() == vec.end()); }
template<typename T> void testIterNotEqual(T &vec)
	{ REQUIRE(vec.begin() != vec.end()); }

TEST_CASE("fixed vector traits", "[fixedVector_t]")
{
	REQUIRE(std::is_move_constructible<fixedVector_t<char>>::value);
	REQUIRE(std::is_move_assignable<fixedVector_t<char>>::value);
	REQUIRE(std::is_default_constructible<fixedVector_t<char>>::value);
	REQUIRE_FALSE(std::is_copy_constructible<fixedVector_t<char>>::value);
	REQUIRE_FALSE(std::is_copy_assignable<fixedVector_t<char>>::value);
}

TEST_CASE("fixed vector invalid", "[fixedVector_t]")
{
	fixedVector_t<int> vec;
	REQUIRE_FALSE(vec.valid());
	REQUIRE_FALSE(bool(vec));
	REQUIRE_FALSE(vec.data());
	REQUIRE(vec.length() == 0);
	REQUIRE(vec.size() == 0);
	REQUIRE(vec.count() == 0);
	testThrowsExcept<fixedVector_t<int>, vectorStateException_t>(vec, "fixedVector_t in invalid state");
	testThrowsExcept<const fixedVector_t<int>, vectorStateException_t>(vec, "fixedVector_t in invalid state");
	testIterEqual<fixedVector_t<int>>(vec);
	testIterEqual<const fixedVector_t<int>>(vec);
}

TEST_CASE("fixed vector indexing", "[fixedVector_t]") try
{
	using fixedVec = fixedVector_t<int>;
	using constFixedVec = const fixedVector_t<int>;

	fixedVec vec(2);
	REQUIRE(vec.valid());
	testIterNotEqual<fixedVector_t<int>>(vec);
	testIterNotEqual<const fixedVector_t<int>>(vec);
	REQUIRE(index<fixedVec>(vec, 0) == 0);
	REQUIRE(index<constFixedVec>(vec, 0) == 0);
	index<fixedVec>(vec, 1) = 5;
	REQUIRE(index<constFixedVec>(vec, 1) == 5);

	testThrowsExcept<fixedVec, std::out_of_range>(vec, "Index into fixedVector_t out of bounds");
	testThrowsExcept<constFixedVec, std::out_of_range>(vec, "Index into fixedVector_t out of bounds");
}
catch (const std::out_of_range &)
	{ FAIL("Unexpected exception thrown during normal fixedVector_t<> access"); }

TEST_CASE("fixed vector swap", "[fixedVector_t]")
{
	fixedVector_t<int> vecA(2), vecB(3);
	REQUIRE(vecA.valid());
	REQUIRE(vecB.valid());
	REQUIRE(vecA.length() == 2);
	REQUIRE(vecB.length() == 3);

	const auto dataA = vecA.data(), dataB = vecB.data();
	swap(vecA, vecB);

	REQUIRE_FALSE(vecA.data() == dataA);
	REQUIRE(vecA.data() == dataB);
	REQUIRE_FALSE(vecB.data() == dataB);
	REQUIRE(vecB.data() == dataA);
	REQUIRE(vecA.length() == 3);
	REQUIRE(vecB.length() == 2);

	fixedVector_t<int> vecC(std::move(vecB));
	REQUIRE_FALSE(vecB.valid());
	REQUIRE(vecC.valid());
	REQUIRE(vecB.length() == 0);
	REQUIRE(vecC.length() == 2);
	REQUIRE(vecC.data() == dataA);

	vecB = std::move(vecC);
	REQUIRE(vecB.valid());
	REQUIRE_FALSE(vecC.valid());
	REQUIRE(vecB.length() == 2);
	REQUIRE(vecC.length() == 0);
	REQUIRE(vecB.data() == dataA);
}
