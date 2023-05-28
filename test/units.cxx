// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <substrate/units>
#include <catch2/catch_test_macros.hpp>

using substrate::operator ""_KiB;
using substrate::operator ""_MiB;
using substrate::operator ""_GiB;
using substrate::operator ""_TiB;
using substrate::operator ""_PiB;

using substrate::operator ""_KB;
using substrate::operator ""_MB;
using substrate::operator ""_GB;
using substrate::operator ""_TB;
using substrate::operator ""_PB;

TEST_CASE("Units", "[utility]")
{
	SECTION("IEC Units")
	{
		REQUIRE(128_KiB == 131072);
		REQUIRE(1024_KiB == 1_MiB);

		REQUIRE(32_MiB == 33554432);
		REQUIRE(1024_MiB == 1_GiB);

		REQUIRE(512_GiB == 549755813888);
		REQUIRE(1024_GiB == 1_TiB);

		REQUIRE(19_TiB == 20890720927744);
		REQUIRE(1024_TiB == 1_PiB);

		REQUIRE(76_PiB == 85568392920039424);

	}

	SECTION("SI Units")
	{
		REQUIRE(128_KB == 128000);
		REQUIRE(1000_KB == 1_MB);

		REQUIRE(32_MB == 32000000);
		REQUIRE(1000_MB == 1_GB);

		REQUIRE(512_GB == 512000000000);
		REQUIRE(1000_GB == 1_TB);

		REQUIRE(19_TB == 19000000000000);
		REQUIRE(1000_TB == 1_PB);

		REQUIRE(76_PB == 76000000000000000);
	}
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
