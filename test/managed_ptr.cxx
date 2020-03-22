#include <cstdint>
#include <substrate/managed_ptr>
#include <catch.hpp>

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
		REQUIRE(bool{value});
		REQUIRE(value.valid());
		REQUIRE((*value).testChar == 'A');
		REQUIRE(value->testNum == 4000);
		REQUIRE(value.get());

		const testType_t &ref = value;
		REQUIRE(&ref == value.get());

		managedPtr_t<testType_t> invalid{};
		REQUIRE_FALSE(bool{invalid});
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
		REQUIRE_FALSE(bool{value});
		REQUIRE(bool{elided});
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
