// SPDX-License-Identifier: BSD-3-Clause
#include <cstdio>
#include <substrate/mmap>
#if !defined(__APPLE__) && !defined(_WIN32)
#include <substrate/memfd>
#endif
#include <substrate/fd>
#include <substrate/pointer_utils>

#include <catch2/catch_test_macros.hpp>
#include <cstring>

using substrate::mmap_t;
#if !defined(__APPLE__) && !defined(_WIN32)
using substrate::memfd_t;
#endif
using substrate::fd_t;
using substrate::nicePtr_t;

#if !defined(__APPLE__) && !defined(_WIN32)
TEST_CASE("Anonymous map test", "[mmap_t]")
{
	memfd_t<4096> mapfd{"mapfd", O_RDWR};
	mmap_t map{mapfd, mapfd.length(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS};
	REQUIRE(map.valid());
}
#endif

struct foo_t final
{
private:
	uint16_t _bar;
	uint32_t _baz;
	uint64_t _quxx;
public:
	constexpr foo_t() noexcept :
		_bar{}, _baz{}, _quxx{} { /* NOP */ }

	foo_t(std::nullptr_t) noexcept { /* NOP */ }

	foo_t(uint16_t bar, uint32_t baz, uint64_t quxx) noexcept :
		_bar{bar}, _baz{baz}, _quxx{quxx} { /* NOP */ }

	void bar(const uint16_t bar) noexcept { _bar = bar; }
	uint16_t bar() const noexcept { return _bar; }

	void baz(const uint32_t baz) noexcept { _baz = baz; }
	uint32_t baz() const noexcept { return _baz; }

	void quxx(const uint64_t quxx) noexcept { _quxx = quxx; }
	uint64_t quxx() const noexcept { return _quxx; }
};

void cleanup(mmap_t *map)
{
	*map = {};
	static_cast<void>(unlink("mmap_t.serialized"));
}

TEST_CASE("Structure serialization and loading", "[mmap_t]")
{
	foo_t a{0, 2, 4};
	foo_t b{8, 16, 32};
	foo_t c{64, 128, 256};
	foo_t d{1024, 2048, 4096};

	SECTION("Serialization")
	{
		fd_t file{"mmap_t.serialized", O_RDWR | O_CREAT | O_NOCTTY, substrate::normalMode};
		REQUIRE(file.valid());
		REQUIRE(file.resize(sizeof(foo_t) * 4));
		for (size_t s{}; s < sizeof(foo_t) * 4; ++s)
			file.write(uint8_t(0x00));
		REQUIRE(file.head());

		auto map = file.map(PROT_READ | PROT_WRITE);
		REQUIRE(map.valid());
		REQUIRE(!file.valid());
		REQUIRE(map.length() == sizeof(foo_t) * 4);

		SUBSTRATE_NOWARN_UNUSED(auto _a) = map.at<foo_t>(0);
		SUBSTRATE_NOWARN_UNUSED(auto _b) = map.at<foo_t>(1);
		SUBSTRATE_NOWARN_UNUSED(auto _c) = map.at<foo_t>(2);
		SUBSTRATE_NOWARN_UNUSED(auto _d) = map.at<foo_t>(3);

		std::memcpy(_a, &a, sizeof(foo_t));
		std::memcpy(_b, &b, sizeof(foo_t));
		std::memcpy(_c, &c, sizeof(foo_t));
		std::memcpy(_d, &d, sizeof(foo_t));

		REQUIRE(map.sync());

		REQUIRE(std::memcmp(_a, &a, sizeof(foo_t)) == 0);
		REQUIRE(std::memcmp(_b, &b, sizeof(foo_t)) == 0);
		REQUIRE(std::memcmp(_c, &c, sizeof(foo_t)) == 0);
		REQUIRE(std::memcmp(_d, &d, sizeof(foo_t)) == 0);
	}

	SECTION("De-serialization")
	{
		fd_t file{"mmap_t.serialized", O_RDONLY | O_NOCTTY};
		REQUIRE(file.valid());
		REQUIRE(file.length() == sizeof(foo_t) * 4);

		nicePtr_t<decltype(&cleanup), &cleanup> map{new mmap_t{file.map(PROT_READ)}};
		REQUIRE(map->valid());
		REQUIRE(!file.valid());
		REQUIRE(map->length() == sizeof(foo_t) * 4);

		SUBSTRATE_NOWARN_UNUSED(auto _a) = map->at<foo_t>(0);
		SUBSTRATE_NOWARN_UNUSED(auto _b) = map->at<foo_t>(1);
		SUBSTRATE_NOWARN_UNUSED(auto _c) = map->at<foo_t>(2);
		SUBSTRATE_NOWARN_UNUSED(auto _d) = map->at<foo_t>(3);

		// REQUIRE(std::memcmp(_a, &a, sizeof(foo_t)) == 0);
		// REQUIRE(std::memcmp(_b, &b, sizeof(foo_t)) == 0);
		// REQUIRE(std::memcmp(_c, &c, sizeof(foo_t)) == 0);
		// REQUIRE(std::memcmp(_d, &d, sizeof(foo_t)) == 0);
	}
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
