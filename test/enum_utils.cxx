// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <string>
#include <vector>
#include <substrate/enum_utils>
#include <catch.hpp>

enum class Flags : uint8_t {
	None   = 0,
	Foo    = 1 << 1,
	Bar    = 1 << 2,
	Baz    = 1 << 3,
	Qux    = 1 << 4,
	Quux   = 1 << 5,
	Corge  = 1 << 6,
	Grault = 1 << 7,
};

using substrate::enumPair_t;
using substrate::extract_flags;
using substrate::extract_flag_pairs;
using substrate::enum_name;
using substrate::enum_value;


namespace substrate {
	template<>
	struct enableEnumBitmask_t<Flags>{
			static constexpr bool enabled = true;
	};
}

const std::array<const enumPair_t<Flags>, 8> flags_s{{
	{ Flags::None,    "No Flags" },
	{ Flags::Foo,     "Foo"      },
	{ Flags::Bar,     "Bar"      },
	{ Flags::Baz,     "Baz"      },
	{ Flags::Qux,     "Qux"      },
	{ Flags::Quux,    "Quux"     },
	{ Flags::Corge,   "Corge"    },
	{ Flags::Grault,  "Grault"   },
}};


TEST_CASE( "Flag extraction", "[enum_utils]")
{
	/* This test isn't the best as it's order-dependent for the flags */
	SECTION( "Raw flag extraction")
	{
		Flags tc1 = Flags::Foo | Flags::Baz | Flags::Grault;
		std::vector<Flags> tc1_exp = { Flags::Foo , Flags::Baz , Flags::Grault };

		auto retvec = extract_flags<Flags>(tc1, flags_s);

		REQUIRE(retvec.size() == tc1_exp.size());
		REQUIRE(retvec == tc1_exp);
	}

	SECTION( "Flag pair extraction")
	{
		Flags tc2 = Flags::Grault | Flags::Quux | Flags::Baz | Flags::Bar;
		std::vector<enumPair_t<Flags>> tc2_exp = {
			{ Flags::Grault,  "Grault"   },
			{ Flags::Quux,    "Quux"     },
			{ Flags::Baz,     "Baz"      },
			{ Flags::Bar,     "Bar"      },
		};

		auto retvec = extract_flag_pairs<Flags>(tc2, flags_s);

		/*
			We need to reverse the returned vector because it's in-order
			while the test vector is inverted.
		*/
		std::reverse(retvec.begin(), retvec.end());

		REQUIRE(retvec.size() == tc2_exp.size());
		REQUIRE(std::equal(retvec.begin(),
					retvec.end(),
					tc2_exp.begin(),
					[](const enumPair_t<Flags>& l, const enumPair_t<Flags>& r){
						return (l.value() == r.value());
					}));
	}

	SECTION ( "Unknown Flag extraction")
	{
		auto ret = extract_flags<Flags>(Flags::None, flags_s);
		auto ret2 = extract_flag_pairs<Flags>(Flags::None, flags_s);

		REQUIRE(ret.size() == 1);
		REQUIRE(ret2.size() == 1);

		REQUIRE(ret[0] == Flags::None);

		REQUIRE(ret2[0].value() == Flags::None);
		REQUIRE(std::string{ret2[0].name()} == std::string{"No Flags"});
	}
}


TEST_CASE( "Enum bitwise operators", "[enum_utils]")
{
	SECTION ( "Bitwise Or")
	{
		REQUIRE(static_cast<uint8_t>(Flags::Foo    | Flags::Quux)  == ((1U << 1U) | (1U << 5U)));
		REQUIRE(static_cast<uint8_t>(Flags::Quux   | Flags::Foo)   == ((1U << 5U) | (1U << 1U)));
		REQUIRE(static_cast<uint8_t>(Flags::Grault | Flags::Corge) == ((1U << 7U) | (1U << 6U)));
		REQUIRE(static_cast<uint8_t>(Flags::Corge  | Flags::None)  == ((1U << 6U) | 0U));

		REQUIRE(static_cast<uint8_t>(Flags::Grault | (1U << 6U)) == ((1U << 7U) | (1U << 6U)));
		REQUIRE(static_cast<uint8_t>((1U << 5U)    | Flags::Foo)   == ((1U << 5U) | (1U << 1U)));


	}

	SECTION ( "Bitwise And")
	{
		REQUIRE(static_cast<uint8_t>(Flags::Foo    & Flags::Quux)  == ((1U << 1) & (1U << 5U)));
		REQUIRE(static_cast<uint8_t>(Flags::Quux   & Flags::Foo)   == ((1U << 5) & (1U << 1U)));
		REQUIRE(static_cast<uint8_t>(Flags::Grault & Flags::Corge) == ((1U << 7) & (1U << 6U)));
		REQUIRE(static_cast<uint8_t>(Flags::Corge  & Flags::None)  == ((1U << 6) & 0U));

		REQUIRE(static_cast<uint8_t>(Flags::Foo    & (1U << 5U))  == ((1U << 1) & (1U << 5U)));
		REQUIRE(static_cast<uint8_t>((1U << 7)     & Flags::Corge) == ((1U << 7) & (1U << 6U)));


	}

	SECTION ( "Bitwise Not")
	{
		REQUIRE(static_cast<uint8_t>(~Flags::Foo   )  == static_cast<uint8_t>(~(1U << 1U)));
		REQUIRE(static_cast<uint8_t>(~Flags::Quux  )  == static_cast<uint8_t>(~(1U << 5U)));
		REQUIRE(static_cast<uint8_t>(~Flags::Grault)  == static_cast<uint8_t>(~(1U << 7U)));
		REQUIRE(static_cast<uint8_t>(~Flags::Corge )  == static_cast<uint8_t>(~(1U << 6U)));
	}

	SECTION ( "Bitwise Xor")
	{
		REQUIRE(static_cast<uint8_t>(Flags::Foo    ^ Flags::Quux)  == ((1U << 1U) ^ (1U << 5U)));
		REQUIRE(static_cast<uint8_t>(Flags::Quux   ^ Flags::Foo)   == ((1U << 5U) ^ (1U << 1U)));
		REQUIRE(static_cast<uint8_t>(Flags::Grault ^ Flags::Corge) == ((1U << 7U) ^ (1U << 6U)));
		REQUIRE(static_cast<uint8_t>(Flags::Corge  ^ Flags::None)  == ((1U << 6U) ^ 0U));

		REQUIRE(static_cast<uint8_t>(Flags::Quux   ^ (1U << 1U))   == ((1U << 5U) ^ (1U << 1U)));
		REQUIRE(static_cast<uint8_t>((1U << 7U)    ^ Flags::Corge) == ((1U << 7U) ^ (1U << 6U)));
	}

	SECTION ( "Bitwise Or Equ")
	{
		Flags f{Flags::Foo};
		uint8_t r{1U << 1U};
		REQUIRE(static_cast<uint8_t>(f |= Flags::Quux) == (r |= (1U << 5U)));
	}

	SECTION ( "Bitwise And Equ")
	{
		Flags f{Flags::Foo};
		uint8_t r{1U << 1U};
		REQUIRE(static_cast<uint8_t>(f &= Flags::Quux) == (r &= (1U << 5U)));
	}

	SECTION ( "Bitwise Xor Equ")
	{
		Flags f{Flags::Foo};
		uint8_t r{1U << 1U};
		REQUIRE(static_cast<uint8_t>(f ^= Flags::Quux) == (r ^= (1U << 5U)));
	}
}

TEST_CASE( "Enum and string mapping", "[enum_utils]")
{
	REQUIRE(enum_name(flags_s, Flags::Quux) == std::string{"Quux"});
	REQUIRE(enum_value(flags_s, "Quux") == Flags::Quux);
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
