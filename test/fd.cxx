#include <fd>
#include <catch.hpp>

TEST_CASE("fd_t bad fd", "[fd_t]")
{
	fd_t file{};
	REQUIRE(file == -1);
	REQUIRE(!file.valid());
	REQUIRE(!file.isEOF());

	REQUIRE(file.read(nullptr, 0, nullptr) == -1);
	REQUIRE(file.write(nullptr, 0) == -1);
}
