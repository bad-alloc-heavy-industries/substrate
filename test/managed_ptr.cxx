#include <cstdint>
#include <substrate/managed_ptr>
#include <catch2/catch.hpp>

using substrate::managedPtr_t;
using substrate::make_managed;
using substrate::make_managed_nothrow;

struct testType_t
{
	char testChar;
	int32_t testNum;
};

TEST_CASE("managedPtr_t<T>", "[managedPtr_t]")
{
	REQUIRE_NOTHROW([]()
	{
		auto value = make_managed<testType_t>('A', 4000);
		REQUIRE(static_cast<bool>(value));
		REQUIRE(value.valid());
		REQUIRE((*value).testChar == 'A');
		REQUIRE(value->testNum == 4000);
		REQUIRE(value.get());

		const testType_t &ref = value;
		REQUIRE(&ref == value.get());

		managedPtr_t<testType_t> invalid{};
		REQUIRE_FALSE(static_cast<bool>(invalid));
		REQUIRE_FALSE(invalid.valid());
		REQUIRE_FALSE(invalid.get());

		value.swap(invalid);
		REQUIRE_FALSE(value.valid());
		REQUIRE_FALSE(value.get());
		REQUIRE(invalid.valid());
		REQUIRE(invalid.get());
		REQUIRE(invalid.get() == &ref);

		value.swap(invalid);
		REQUIRE_FALSE(invalid.valid());
		REQUIRE(value.valid());

		managedPtr_t<void> elided{std::move(value)};
		REQUIRE_FALSE(static_cast<bool>(value));
		REQUIRE(static_cast<bool>(elided));
		REQUIRE(elided);
		void *const ptr = elided;
		REQUIRE(elided.get() == ptr);
		const void *const constPtr = elided;
		REQUIRE(elided.get() == constPtr);
		REQUIRE(elided.get<testType_t>() == &ref);
	}());

	auto value = make_managed_nothrow<char>('B');
	REQUIRE(value.valid());
	REQUIRE(*value == 'B');
}

TEST_CASE("managedPtr_t<T []>", "managedPtr_t")
{
	REQUIRE_NOTHROW([]()
	{
		auto value = make_managed<int32_t []>(2);
		REQUIRE(static_cast<bool>(value));
		REQUIRE(value.valid());
		REQUIRE_FALSE(value[0]);
		value[0] = 42;
		REQUIRE_FALSE(value[1]);
		value[1] = 4000;

		REQUIRE(*value == 42);
		REQUIRE(value.get());

		const int32_t &ref = value;
		REQUIRE(&ref == value.get());

		managedPtr_t<int32_t []> invalid{};
		REQUIRE_FALSE(static_cast<bool>(invalid));
		REQUIRE_FALSE(invalid.valid());
		REQUIRE_FALSE(invalid.get());

		value.swap(invalid);
		REQUIRE_FALSE(value.valid());
		REQUIRE_FALSE(value.get());
		REQUIRE(invalid.valid());
		REQUIRE(invalid.get());
		REQUIRE(invalid.get() == &ref);

		value.swap(invalid);
		REQUIRE_FALSE(invalid.valid());
		REQUIRE(value.valid());

		managedPtr_t<void> elided{std::move(value)};
		REQUIRE_FALSE(static_cast<bool>(value));
		REQUIRE(static_cast<bool>(elided));
		REQUIRE(elided);
		void *const ptr = elided;
		REQUIRE(elided.get() == ptr);
		const void *const constPtr = elided;
		REQUIRE(elided.get() == constPtr);
		REQUIRE(elided.get<int32_t>() == &ref);
	}());

	auto value = make_managed_nothrow<int32_t []>(1);
	REQUIRE(value.valid());
	REQUIRE_FALSE(value[0]);
	value[0] = 32768;
	REQUIRE(*value == 32768);
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
