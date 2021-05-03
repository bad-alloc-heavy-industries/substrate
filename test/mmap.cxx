// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/mmap>
#include <substrate/memfd>
#include <substrate/fd>

#include <catch.hpp>
#include <cstring>

using substrate::mmap_t;
using substrate::memfd_t;
using substrate::fd_t;

TEST_CASE("Anonymous map test", "[mmap_t]")
{
	memfd_t<4096> mapfd{"mapfd", O_RDWR};
	mmap_t map{mapfd, mapfd.length(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS};
	REQUIRE(map.valid());
}

struct foo final
{
private:
	uint16_t _bar;
	uint32_t _baz;
	uint64_t _quxx;
public:
	constexpr foo() noexcept :
		_bar{}, _baz{}, _quxx{} { /* NOP */ }

	foo(std::nullptr_t) noexcept { /* NOP */ }

	foo(uint16_t bar, uint32_t baz, uint64_t quxx) noexcept :
		_bar{bar}, _baz{baz}, _quxx{quxx} { /* NOP */ }

	void bar(const uint16_t bar) noexcept { _bar = bar; }
	uint16_t bar() const noexcept { return _bar; }

	void baz(const uint32_t baz) noexcept { _baz = baz; }
	uint32_t baz() const noexcept { return _baz; }

	void quxx(const uint64_t quxx) noexcept { _quxx = quxx; }
	uint64_t quxx() const noexcept { return _quxx; }
};

TEST_CASE("Structure serialization and loading", "[mmap_t]")
{
	foo a{0, 2, 4};
	foo b{8, 16, 32};
	foo c{64, 128, 256};
	foo d{1024, 2048, 4096};

	SECTION("Serialization")
	{
		fd_t file{"mmap_t.serialized", O_RDWR | O_CREAT, substrate::normalMode};
		REQUIRE(file.valid());
		REQUIRE(file.resize(sizeof(foo) * 4));
		for (size_t s{}; s < sizeof(foo) * 4; ++s)
			file.write(uint8_t(0x00));
		REQUIRE(file.head());

		auto mp = file.map(PROT_READ | PROT_WRITE);
		REQUIRE(mp.valid());
		REQUIRE(!file.valid());
		REQUIRE(mp.length() == sizeof(foo) * 4);

		SUBSTRATE_NOWARN_UNUSED(auto _a) = mp.at<foo>(0);
		SUBSTRATE_NOWARN_UNUSED(auto _b) = mp.at<foo>(1);
		SUBSTRATE_NOWARN_UNUSED(auto _c) = mp.at<foo>(2);
		SUBSTRATE_NOWARN_UNUSED(auto _d) = mp.at<foo>(3);

		std::memcpy(_a, &a, sizeof(foo));
		std::memcpy(_b, &b, sizeof(foo));
		std::memcpy(_c, &c, sizeof(foo));
		std::memcpy(_d, &d, sizeof(foo));

		REQUIRE(mp.sync());

		REQUIRE(std::memcmp(_a, &a, sizeof(foo)) == 0);
		REQUIRE(std::memcmp(_b, &b, sizeof(foo)) == 0);
		REQUIRE(std::memcmp(_c, &c, sizeof(foo)) == 0);
		REQUIRE(std::memcmp(_d, &d, sizeof(foo)) == 0);
	}

	SECTION("De-serialization")
	{
		fd_t file{"mmap_t.serialized", O_RDONLY, substrate::normalMode};
		REQUIRE(file.valid());
		REQUIRE(file.length() == sizeof(foo) * 4);

		auto mp = file.map(PROT_READ);
		REQUIRE(mp.valid());
		REQUIRE(!file.valid());
		REQUIRE(mp.length() == sizeof(foo) * 4);

		SUBSTRATE_NOWARN_UNUSED(auto _a) = mp.at<foo>(0);
		SUBSTRATE_NOWARN_UNUSED(auto _b) = mp.at<foo>(1);
		SUBSTRATE_NOWARN_UNUSED(auto _c) = mp.at<foo>(2);
		SUBSTRATE_NOWARN_UNUSED(auto _d) = mp.at<foo>(3);

		// REQUIRE(std::memcmp(_a, &a, sizeof(foo)) == 0);
		// REQUIRE(std::memcmp(_b, &b, sizeof(foo)) == 0);
		// REQUIRE(std::memcmp(_c, &c, sizeof(foo)) == 0);
		// REQUIRE(std::memcmp(_d, &d, sizeof(foo)) == 0);
	}



}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
