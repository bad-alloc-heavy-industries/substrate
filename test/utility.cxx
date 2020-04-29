// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <string>
#include <random>
#include <exception>

#if __cplusplus < 201402L && !defined(SUBSTRATE_CXX11_COMPAT)
#	define SUBSTRATE_CXX11_COMPAT
#endif
#include <substrate/utility>
#include <catch.hpp>

namespace test
{
	class A {};
	enum B : int {};
	enum struct C : char {};
	union D { int a; float b; };
	struct E { D c; };
	class F : A {};
	enum class G {};

	struct H
	{
		int foo() const &;
	};

	class I : A {};
	class J : I {};
	class K {};
	class L : public A {};

	struct M
	{
		A a;
		operator A() { return a; }
	};

	struct N
	{
		N() = default;
		N(const N &) = delete;
		N(N &&) = delete;
		virtual ~N() = 0;
		N &operator =(const N &) = delete;
		N &operator =(N &&) = delete;
	};

	struct O
	{
		O() = default;
		O(const O &) = delete;
		O(O &&) = delete;
		virtual ~O() = default;
		O &operator =(const O &) = delete;
		O &operator =(O &&) = delete;

		virtual void baz();
	};

	struct P
	{
		P() = default;
		P(const P &) = default;
		P(P &&) = default;
		~P() noexcept = default;
		P &operator =(const P &) = default;
		P &operator =(P &&) = default;
	};

	struct Q
	{
		std::string foo;

		Q() = default;
		Q(const Q &) = default;
		Q(Q &&) = default;
		~Q() noexcept = default;
		Q &operator =(const Q &) = default;
		Q &operator =(Q &&) = default;
	};

	struct R
	{
		R() = default;
		R(const R &) = default;
		R(R &&) = default;
		~R() noexcept = default;
		R &operator =(const R &) { return *this; } // NOLINT(cert-oop54-cpp,modernize-use-equals-default)
		R &operator =(R &&) { return *this; } // NOLINT(performance-noexcept-move-constructor)
	};

	struct S { int foo; };
	struct T { std::string foo; };

	struct U
	{
		int bar{};

		U(int baz)
		{
			if (!baz)
				throw baz;
		}
	};

	struct V { U qux; };

	struct W
	{
		W() { throw std::exception{}; }
		W(const W &) { throw std::exception{}; } // NOLINT(cert-err09-cpp)
		W(W &&) { throw std::exception{}; } // NOLINT(cert-err09-cpp,bugprone-exception-escape,performance-noexcept-move-constructor)
		~W() noexcept = default;
		W &operator =(const W &) = default;
		W &operator =(W &&) = default;
	};

	struct X { W qux; };

	class Y
	{
		int foo; // NOLINT(clang-diagnostic-unused-private-field)
		double bar; // NOLINT(clang-diagnostic-unused-private-field)

	public:
		Y(int baz) : foo{baz}, bar{} { }
		Y(int qux, double fwibble) noexcept : foo{qux}, bar{fwibble} { }
	};

	struct Z
	{
		int bar;
	private:
		int baz; // NOLINT(clang-diagnostic-unused-private-field)
	};

	struct AA
	{
		int foo;

		AA() = default;
		AA(const AA &) = default;
		AA(AA &&) = delete;
		~AA() = default;
		AA &operator =(const AA &) = default;
		AA &operator =(AA &&) = delete;
	};

	struct AB
	{
		AB() = default;
		AB(const AB &) = delete;
		AB(AB &&) = delete;
		virtual ~AB() = default;
		AB &operator =(const AB &) = delete;
		AB &operator =(AB &&) = delete;

		virtual void baz() = 0;
	};

	struct AC : AB {};
	struct AD : O {};
	struct AE {};
	union AF {};

	struct AG
	{
		AG() = default;
		AG(const AG &) = delete;
		AG(AG &&) = delete;
		virtual ~AG() = default;
		AG &operator =(const AG &) = delete;
		AG &operator =(AG &&) = delete;
	};

	struct AH
	{
		int foo;

		AH() = default;
		AH(int qux) : foo{qux + 1} { }
		AH(const AH &) = default;
		AH(AH &&) = default;
		~AH() = default;
		AH &operator =(const AH &) = default;
		AH &operator =(AH &&) = default;
	};

	struct AI { void foo() { } };

	struct AJ
	{
		AJ() = delete;
		AJ(const AJ &) = default;
		AJ(AJ &&) = default;
		~AJ() = default;
		AJ &operator =(const AJ &) = default;
		AJ &operator =(AJ &&) = default;
	};


	struct AK
	{
	private:
		int foo;
		int bar;
	public:

		constexpr AK(std::nullptr_t) noexcept :
			foo{}, bar{} { }
	};

	int bar();

	template<typename> struct SU_traits {};
	template<class T, class U> struct SU_traits<U T::*>
		{ using member_type = U; };

	struct SU_is_empty_v { static int bar; };
}

/* C++ 17 helpers */
#if __cplusplus < 201703L && __cplusplus >= 201402L

using substrate::is_same_v;
TEST_CASE("[C++ 17] is_same_v helper", "[utility]")
{
	using uint = unsigned int;
	using sint = signed int;

	REQUIRE_FALSE(is_same_v<int, bool>);
	REQUIRE_FALSE(is_same_v<float, int>);
	REQUIRE_FALSE(is_same_v<test::A, int>);
	REQUIRE_FALSE(is_same_v<uint, sint>);

	REQUIRE(is_same_v<test::A, test::A>);
	REQUIRE(is_same_v<uint, unsigned int>);
	REQUIRE(is_same_v<sint, signed int>);
}

using substrate::is_null_pointer_v;
TEST_CASE("[C++ 17] is_null_pointer_v helper", "[utility]")
{
	REQUIRE(is_null_pointer_v<decltype(nullptr)>);
	REQUIRE_FALSE(is_null_pointer_v<int*>);
}

using substrate::is_void_v;
TEST_CASE("[C++ 17] is_void_v helper", "[utility]")
{
	REQUIRE(is_void_v<void>);
	REQUIRE_FALSE(is_void_v<int>);
}

using substrate::is_integral_v;
TEST_CASE("[C++ 17] is_integral_v helper", "[utility]")
{
	REQUIRE_FALSE(is_integral_v<test::A>);
	REQUIRE_FALSE(is_integral_v<test::B>);
	REQUIRE_FALSE(is_integral_v<float>);
	REQUIRE_FALSE(is_integral_v<double>);

	REQUIRE(is_integral_v<int>);
	REQUIRE(is_integral_v<bool>);
	REQUIRE(is_integral_v<unsigned int>);
	REQUIRE(is_integral_v<unsigned char>);

}

using substrate::is_floating_point_v;
TEST_CASE("[C++ 17] is_floating_point_v helper", "[utility]")
{
	REQUIRE_FALSE(is_floating_point_v<test::A>);
	REQUIRE_FALSE(is_floating_point_v<test::B>);
	REQUIRE_FALSE(is_floating_point_v<int>);
	REQUIRE_FALSE(is_floating_point_v<bool>);
	REQUIRE_FALSE(is_floating_point_v<unsigned int>);
	REQUIRE_FALSE(is_floating_point_v<unsigned char>);

	REQUIRE(is_floating_point_v<float>);
	REQUIRE(is_floating_point_v<double>);
}

using substrate::is_array_v;
TEST_CASE("[C++ 17] is_array_v helper", "[utility]")
{
	REQUIRE_FALSE(is_array_v<test::A>);
	REQUIRE_FALSE(is_array_v<test::B>);
	REQUIRE_FALSE(is_array_v<int>);
	REQUIRE_FALSE(is_array_v<bool>);

	REQUIRE(is_array_v<test::A []>); // NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
	REQUIRE(is_array_v<test::A [2]>); // NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
	REQUIRE(is_array_v<test::B []>); // NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
	REQUIRE(is_array_v<test::B [4]>); // NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
	REQUIRE(is_array_v<int []>); // NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
	REQUIRE(is_array_v<int [8]>); // NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
	REQUIRE(is_array_v<bool []>); // NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
	REQUIRE(is_array_v<bool [16]>); // NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
}

using substrate::is_enum_v;
TEST_CASE("[C++ 17] is_enum_v helper", "[utility]")
{
	REQUIRE_FALSE(is_enum_v<test::A>);
	REQUIRE_FALSE(is_enum_v<int>);
	REQUIRE_FALSE(is_enum_v<bool>);

	REQUIRE(is_enum_v<test::B>);
	REQUIRE(is_enum_v<test::C>);

}

using substrate::is_union_v;
TEST_CASE("[C++ 17] is_union_v helper", "[utility]")
{
	REQUIRE_FALSE(is_union_v<test::A>);
	REQUIRE_FALSE(is_union_v<test::B>);
	REQUIRE_FALSE(is_union_v<test::E>);
	REQUIRE_FALSE(is_union_v<int>);
	REQUIRE_FALSE(is_union_v<void>);

	REQUIRE(is_union_v<test::D>);
}

using substrate::is_class_v;
TEST_CASE("[C++ 17] is_class_v helper", "[utility]")
{
	REQUIRE_FALSE(is_class_v<test::B>);
	REQUIRE_FALSE(is_class_v<test::D>);
	REQUIRE_FALSE(is_class_v<test::G>);
	REQUIRE_FALSE(is_class_v<int>);
	REQUIRE_FALSE(is_class_v<void>);

	REQUIRE(is_class_v<test::A>);
	REQUIRE(is_class_v<test::E>);
	REQUIRE(is_class_v<test::F>);
}

using substrate::is_function_v;
TEST_CASE("[C++ 17] is_function_v helper", "[utility]")
{
	REQUIRE_FALSE(is_function_v<test::A>);
	REQUIRE_FALSE(is_function_v<test::B>);
	REQUIRE_FALSE(is_function_v<test::H>);
	REQUIRE_FALSE(is_function_v<int>);
	REQUIRE_FALSE(is_function_v<void>);

	REQUIRE(is_function_v<int(void)>);
	REQUIRE(is_function_v<void(int)>);
	REQUIRE(is_function_v<decltype(test::bar)>);
	REQUIRE(is_function_v<test::SU_traits<decltype(&test::H::foo)>::member_type>);
}

using substrate::is_pointer_v;
TEST_CASE("[C++ 17] is_pointer_v helper", "[utility]")
{
	REQUIRE_FALSE(is_pointer_v<test::A>);
	REQUIRE_FALSE(is_pointer_v<test::B &>);
	REQUIRE_FALSE(is_pointer_v<int>);
	REQUIRE_FALSE(is_pointer_v<void>);
	REQUIRE_FALSE(is_pointer_v<char []>); // NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
	REQUIRE_FALSE(is_pointer_v<bool [1]>); // NOLINT(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
	REQUIRE_FALSE(is_pointer_v<std::nullptr_t>);

	REQUIRE(is_pointer_v<test::A *>);
	REQUIRE(is_pointer_v<int *>);
	REQUIRE(is_pointer_v<void **>);
}

using substrate::is_lvalue_reference_v;
TEST_CASE("[C++ 17] is_lvalue_reference_v helper", "[utility]")
{
	REQUIRE_FALSE(is_lvalue_reference_v<test::A>);
	REQUIRE_FALSE(is_lvalue_reference_v<test::A &&>);
	REQUIRE_FALSE(is_lvalue_reference_v<int>);
	REQUIRE_FALSE(is_lvalue_reference_v<int &&>);

	REQUIRE(is_lvalue_reference_v<test::A &>);
	REQUIRE(is_lvalue_reference_v<int &>);
}

using substrate::is_rvalue_reference_v;
TEST_CASE("[C++ 17] is_rvalue_reference_v helper", "[utility]")
{
	REQUIRE_FALSE(is_rvalue_reference_v<test::A>);
	REQUIRE_FALSE(is_rvalue_reference_v<test::A &>);
	REQUIRE_FALSE(is_rvalue_reference_v<int>);
	REQUIRE_FALSE(is_rvalue_reference_v<int &>);

	REQUIRE(is_rvalue_reference_v<test::A &&>);
	REQUIRE(is_rvalue_reference_v<int &&>);
}

using substrate::is_member_object_pointer_v;
TEST_CASE("[C++ 17] is_member_object_pointer_v helper", "[utility]")
{
	REQUIRE_FALSE(is_member_object_pointer_v<int(test::A::*)()>);

	REQUIRE(is_member_object_pointer_v<int(test::A::*)>);
}

using substrate::is_member_function_pointer_v;
TEST_CASE("[C++ 17] is_member_function_pointer_v helper", "[utility]")
{
	REQUIRE_FALSE(is_member_function_pointer_v<decltype(test::bar)>);

	REQUIRE(is_member_function_pointer_v<decltype(&test::AI::foo)>);
}

using substrate::is_fundamental_v;
TEST_CASE("[C++ 17] is_fundamental_v helper", "[utility]")
{
	REQUIRE_FALSE(is_fundamental_v<test::A>);
	REQUIRE_FALSE(is_fundamental_v<test::A *>);
	REQUIRE_FALSE(is_fundamental_v<test::A &>);
	REQUIRE_FALSE(is_fundamental_v<test::A &&>);
	REQUIRE_FALSE(is_fundamental_v<int *>);
	REQUIRE_FALSE(is_fundamental_v<int &>);
	REQUIRE_FALSE(is_fundamental_v<int &&>);
	REQUIRE_FALSE(is_fundamental_v<decltype(test::bar)>);

	REQUIRE(is_fundamental_v<int>);
	REQUIRE(is_fundamental_v<float>);
	REQUIRE(is_fundamental_v<bool>);
}

using substrate::is_arithmetic_v;
TEST_CASE("[C++ 17] is_arithmetic_v helper", "[utility]")
{
	REQUIRE_FALSE(is_arithmetic_v<test::A>);
	REQUIRE_FALSE(is_arithmetic_v<test::B>);
	REQUIRE_FALSE(is_arithmetic_v<int *>);
	REQUIRE_FALSE(is_arithmetic_v<int &>);
	REQUIRE_FALSE(is_arithmetic_v<float *>);
	REQUIRE_FALSE(is_arithmetic_v<float &>);

	REQUIRE(is_arithmetic_v<int>);
	REQUIRE(is_arithmetic_v<int const>);
	REQUIRE(is_arithmetic_v<float>);
	REQUIRE(is_arithmetic_v<float const>);
	REQUIRE(is_arithmetic_v<char>);
	REQUIRE(is_arithmetic_v<char const>);
	REQUIRE(is_arithmetic_v<bool>);
	REQUIRE(is_arithmetic_v<bool const>);
}

using substrate::is_object_v;
TEST_CASE("[C++ 17] is_object_v helper", "[utility]")
{
	REQUIRE_FALSE(is_object_v<test::A &>);
	REQUIRE_FALSE(is_object_v<test::A &&>);
	REQUIRE_FALSE(is_object_v<test::B &>);
	REQUIRE_FALSE(is_object_v<test::B &&>);
	REQUIRE_FALSE(is_object_v<int &>);
	REQUIRE_FALSE(is_object_v<int &&>);

	REQUIRE(is_object_v<test::A>);
	REQUIRE(is_object_v<test::B>);
	REQUIRE(is_object_v<int>);
	REQUIRE(is_object_v<float>);
}

using substrate::is_compound_v;
TEST_CASE("[C++ 17] is_compound_v helper", "[utility]")
{
	REQUIRE_FALSE(is_compound_v<int>);

	REQUIRE(is_compound_v<test::A>);
}

using substrate::is_reference_v;
TEST_CASE("[C++ 17] is_reference_v helper", "[utility]")
{
	REQUIRE_FALSE(is_reference_v<test::A>);
	REQUIRE_FALSE(is_reference_v<int>);

	REQUIRE(is_reference_v<test::A &>);
	REQUIRE(is_reference_v<test::A &&>);
	REQUIRE(is_reference_v<int &>);
	REQUIRE(is_reference_v<int &&>);
}

using substrate::is_member_pointer_v;
TEST_CASE("[C++ 17] is_member_pointer_v helper", "[utility]")
{
	REQUIRE_FALSE(is_member_pointer_v<int>);

	REQUIRE(is_member_pointer_v<int(test::A::*)>);
}

using substrate::is_scalar_v;
TEST_CASE("[C++ 17] is_scalar_v helper", "[utility]")
{
	REQUIRE_FALSE(is_scalar_v<test::A>);

	REQUIRE(is_scalar_v<test::B>);
	REQUIRE(is_scalar_v<int>);
	REQUIRE(is_scalar_v<float>);
	REQUIRE(is_scalar_v<bool>);
	REQUIRE(is_scalar_v<char>);
}

using substrate::is_base_of_v;
TEST_CASE("[C++ 17] is_base_of_v helper", "[utility]")
{
	REQUIRE_FALSE(is_base_of_v<test::K, test::A>);
	REQUIRE_FALSE(is_base_of_v<test::I, test::A>);
	REQUIRE_FALSE(is_base_of_v<test::J, test::A>);
	REQUIRE_FALSE(is_base_of_v<test::J, test::I>);

	REQUIRE(is_base_of_v<test::A, test::I>);
	REQUIRE(is_base_of_v<test::A, test::J>);
	REQUIRE(is_base_of_v<test::I, test::J>);
}

using substrate::is_convertible_v;
TEST_CASE("[C++ 17] is_convertible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_convertible_v<test::A, test::L>);
	REQUIRE_FALSE(is_convertible_v<test::L, test::M>);

	REQUIRE(is_convertible_v<test::L, test::A>);
}

using substrate::has_virtual_destructor_v;
TEST_CASE("[C++ 17] has_virtual_destructor_v helper", "[utility]")
{
	REQUIRE_FALSE(has_virtual_destructor_v<test::P>);

	REQUIRE(has_virtual_destructor_v<test::N>);
	REQUIRE(has_virtual_destructor_v<test::O>);
}

using substrate::is_destructible_v;
TEST_CASE("[C++ 17] is_destructible_v helper", "[utility]")
{
	REQUIRE(is_destructible_v<test::P>);
	REQUIRE(is_destructible_v<test::Q>);
}

using substrate::is_trivially_destructible_v;
TEST_CASE("[C++ 17] is_trivially_destructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_destructible_v<test::Q>);

	REQUIRE(is_trivially_destructible_v<test::P>);
}

using substrate::is_nothrow_destructible_v;
TEST_CASE("[C++ 17] is_nothrow_destructible_v helper", "[utility]")
{
	REQUIRE(is_nothrow_destructible_v<test::P>);
	REQUIRE(is_nothrow_destructible_v<test::Q>);
}

using substrate::is_move_assignable_v;
TEST_CASE("[C++ 17] is_move_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_move_assignable_v<int []>);
	REQUIRE_FALSE(is_move_assignable_v<int [4]>);

	REQUIRE(is_move_assignable_v<test::A>);
	REQUIRE(is_move_assignable_v<test::R>);
	REQUIRE(is_move_assignable_v<test::S>);
	REQUIRE(is_move_assignable_v<int>);
}

using substrate::is_trivially_move_assignable_v;
TEST_CASE("[C++ 17] is_trivially_move_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_move_assignable_v<test::R>);
	REQUIRE_FALSE(is_trivially_move_assignable_v<int []>);
	REQUIRE_FALSE(is_trivially_move_assignable_v<int [4]>);

	REQUIRE(is_trivially_move_assignable_v<test::A>);
	REQUIRE(is_trivially_move_assignable_v<test::S>);
	REQUIRE(is_trivially_move_assignable_v<int>);
}

using substrate::is_nothrow_move_assignable_v;
TEST_CASE("[C++ 17] is_nothrow_move_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_move_assignable_v<test::R>);
	REQUIRE_FALSE(is_nothrow_move_assignable_v<int []>);
	REQUIRE_FALSE(is_nothrow_move_assignable_v<int [4]>);

	REQUIRE(is_nothrow_move_assignable_v<test::A>);
	REQUIRE(is_nothrow_move_assignable_v<test::S>);
	REQUIRE(is_nothrow_move_assignable_v<int>);
}

using substrate::is_copy_assignable_v;
TEST_CASE("[C++ 17] is_copy_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_copy_assignable_v<int []>);
	REQUIRE_FALSE(is_copy_assignable_v<int [4]>);

	REQUIRE(is_copy_assignable_v<test::A>);
	REQUIRE(is_copy_assignable_v<test::R>);
	REQUIRE(is_copy_assignable_v<test::S>);
	REQUIRE(is_copy_assignable_v<int>);
}

using substrate::is_trivially_copy_assignable_v;
TEST_CASE("[C++ 17] is_trivially_copy_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_copy_assignable_v<test::R>);
	REQUIRE_FALSE(is_trivially_copy_assignable_v<int []>);
	REQUIRE_FALSE(is_trivially_copy_assignable_v<int [4]>);

	REQUIRE(is_trivially_copy_assignable_v<test::A>);
	REQUIRE(is_trivially_copy_assignable_v<test::S>);
	REQUIRE(is_trivially_copy_assignable_v<int>);
}

using substrate::is_nothrow_copy_assignable_v;
TEST_CASE("[C++ 17] is_nothrow_copy_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_copy_assignable_v<test::R>);
	REQUIRE_FALSE(is_nothrow_copy_assignable_v<int []>);
	REQUIRE_FALSE(is_nothrow_copy_assignable_v<int [4]>);

	REQUIRE(is_nothrow_copy_assignable_v<test::S>);
	REQUIRE(is_nothrow_copy_assignable_v<int>);
}

using substrate::is_assignable_v;
TEST_CASE("[C++ 17] is_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_assignable_v<test::S, int>);
	REQUIRE_FALSE(is_assignable_v<test::V, int>);
	REQUIRE_FALSE(is_assignable_v<int, int>);
	REQUIRE_FALSE(is_assignable_v<int, int &>);
	REQUIRE_FALSE(is_assignable_v<int, float>);

	REQUIRE(is_assignable_v<test::S &, test::S &>);
	REQUIRE(is_assignable_v<test::U, int>);
	REQUIRE(is_assignable_v<int &, int>);
	REQUIRE(is_assignable_v<int &, int &>);
	REQUIRE(is_assignable_v<int &, float>);
}

using substrate::is_trivially_assignable_v;
TEST_CASE("[C++ 17] is_trivially_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_assignable_v<test::S, int>);
	REQUIRE_FALSE(is_trivially_assignable_v<test::U, int>);
	REQUIRE_FALSE(is_trivially_assignable_v<test::V, int>);
	REQUIRE_FALSE(is_trivially_assignable_v<int, int>);
	REQUIRE_FALSE(is_trivially_assignable_v<int, int&>);
	REQUIRE_FALSE(is_trivially_assignable_v<int, float>);

	REQUIRE(is_trivially_assignable_v<test::S &, test::S &>);
	REQUIRE(is_trivially_assignable_v<int &, int>);
	REQUIRE(is_trivially_assignable_v<int &, int &>);
	REQUIRE(is_trivially_assignable_v<int &, float>);
}

using substrate::is_nothrow_assignable_v;
TEST_CASE("[C++ 17] is_nothrow_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_assignable_v<test::S, int>);
	REQUIRE_FALSE(is_nothrow_assignable_v<test::U, int>);
	REQUIRE_FALSE(is_nothrow_assignable_v<test::V, int>);
	REQUIRE_FALSE(is_nothrow_assignable_v<int, int>);
	REQUIRE_FALSE(is_nothrow_assignable_v<int, int &>);
	REQUIRE_FALSE(is_nothrow_assignable_v<int, float>);

	REQUIRE(is_nothrow_assignable_v<test::S &, test::S &>);
	REQUIRE(is_nothrow_assignable_v<int &, int>);
	REQUIRE(is_nothrow_assignable_v<int &, int &>);
	REQUIRE(is_nothrow_assignable_v<int &, float>);
}

using substrate::is_move_constructible_v;
TEST_CASE("[C++ 17] is_move_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_move_constructible_v<test::N>);

	REQUIRE(is_move_constructible_v<test::S>);
	REQUIRE(is_move_constructible_v<test::T>);
	REQUIRE(is_move_constructible_v<test::W>);
	REQUIRE(is_move_constructible_v<test::X>);
}

using substrate::is_trivially_move_constructible_v;
TEST_CASE("[C++ 17] is_trivially_move_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_move_constructible_v<test::N>);
	REQUIRE_FALSE(is_trivially_move_constructible_v<test::T>);
	REQUIRE_FALSE(is_trivially_move_constructible_v<test::W>);
	REQUIRE_FALSE(is_trivially_move_constructible_v<test::X>);

	REQUIRE(is_trivially_move_constructible_v<test::S>);
}

using substrate::is_nothrow_move_constructible_v;
TEST_CASE("[C++ 17] is_nothrow_move_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_move_constructible_v<test::N>);
	REQUIRE_FALSE(is_nothrow_move_constructible_v<test::W>);
	REQUIRE_FALSE(is_nothrow_move_constructible_v<test::X>);

	REQUIRE(is_nothrow_move_constructible_v<test::S>);
	REQUIRE(is_nothrow_move_constructible_v<test::T>);
}

using substrate::is_copy_constructible_v;
TEST_CASE("[C++ 17] is_copy_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_copy_constructible_v<test::N>);

	REQUIRE(is_copy_constructible_v<test::S>);
	REQUIRE(is_copy_constructible_v<test::T>);
	REQUIRE(is_copy_constructible_v<test::W>);
	REQUIRE(is_copy_constructible_v<test::X>);
}

using substrate::is_trivially_copy_constructible_v;
TEST_CASE("[C++ 17] is_trivially_copy_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_copy_constructible_v<test::N>);
	REQUIRE_FALSE(is_trivially_copy_constructible_v<test::T>);
	REQUIRE_FALSE(is_trivially_copy_constructible_v<test::W>);
	REQUIRE_FALSE(is_trivially_copy_constructible_v<test::X>);

	REQUIRE(is_trivially_copy_constructible_v<test::S>);
}

using substrate::is_nothrow_copy_constructible_v;
TEST_CASE("[C++ 17] is_nothrow_copy_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<test::N>);
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<test::T>);
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<test::W>);
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<test::X>);

	REQUIRE(is_nothrow_copy_constructible_v<test::S>);
}

using substrate::is_default_constructible_v;
TEST_CASE("[C++ 17] is_default_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_default_constructible_v<test::N>);
	REQUIRE_FALSE(is_default_constructible_v<test::U>);
	REQUIRE_FALSE(is_default_constructible_v<test::V>);
	REQUIRE_FALSE(is_default_constructible_v<test::AJ>);

	REQUIRE(is_default_constructible_v<test::T>);
	REQUIRE(is_default_constructible_v<test::S>);
}

using substrate::is_trivially_default_constructible_v;
TEST_CASE("[C++ 17] is_trivially_default_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_default_constructible_v<test::N>);
	REQUIRE_FALSE(is_trivially_default_constructible_v<test::T>);
	REQUIRE_FALSE(is_trivially_default_constructible_v<test::U>);
	REQUIRE_FALSE(is_trivially_default_constructible_v<test::V>);
	REQUIRE_FALSE(is_trivially_default_constructible_v<test::AJ>);

	REQUIRE(is_trivially_default_constructible_v<test::S>);
}

using substrate::is_nothrow_default_constructible_v;
TEST_CASE("[C++ 17] is_nothrow_default_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_default_constructible_v<test::N>);
	REQUIRE_FALSE(is_nothrow_default_constructible_v<test::U>);
	REQUIRE_FALSE(is_nothrow_default_constructible_v<test::V>);
	REQUIRE_FALSE(is_nothrow_default_constructible_v<test::AJ>);

	REQUIRE(is_nothrow_default_constructible_v<test::T>);
	REQUIRE(is_nothrow_default_constructible_v<test::S>);
}

using substrate::is_constructible_v;
TEST_CASE("[C++ 17] is_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_constructible_v<test::Y, test::A>);

	REQUIRE(is_constructible_v<test::Y, int>);
	REQUIRE(is_constructible_v<test::Y, int, double>);
}

using substrate::is_trivially_constructible_v;
TEST_CASE("[C++ 17] is_trivially_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_constructible_v<test::Y, int>);

	REQUIRE(is_trivially_constructible_v<test::Y, const test::Y &>);
}

using substrate::is_nothrow_constructible_v;
TEST_CASE("[C++ 17] is_nothrow_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_constructible_v<test::Y, int>);

	REQUIRE(is_nothrow_constructible_v<test::Y, int, double>);
}

using substrate::is_unsigned_v;
TEST_CASE("[C++ 17] is_unsigned_v helper", "[utility]")
{
	enum struct C : int {};
	enum D : unsigned int {};
	enum struct E : unsigned int {};

	REQUIRE_FALSE(is_unsigned_v<test::A>);
	REQUIRE_FALSE(is_unsigned_v<test::B>);
	REQUIRE_FALSE(is_unsigned_v<C>);
	REQUIRE_FALSE(is_unsigned_v<D>);
	REQUIRE_FALSE(is_unsigned_v<E>);
	REQUIRE_FALSE(is_unsigned_v<int>);
	REQUIRE_FALSE(is_unsigned_v<signed int>);

	REQUIRE(is_unsigned_v<unsigned int>);
}

using substrate::is_signed_v;
TEST_CASE("[C++ 17] is_signed_v helper", "[utility]")
{
	enum struct C : int {};
	enum D : unsigned int {};
	enum struct E : unsigned int {};

	REQUIRE_FALSE(is_signed_v<test::A>);
	REQUIRE_FALSE(is_signed_v<test::B>);
	REQUIRE_FALSE(is_signed_v<C>);
	REQUIRE_FALSE(is_signed_v<D>);
	REQUIRE_FALSE(is_signed_v<E>);
	REQUIRE_FALSE(is_signed_v<unsigned int>);

	REQUIRE(is_signed_v<int>);
	REQUIRE(is_signed_v<signed int>);
}

using substrate::is_abstract_v;
TEST_CASE("[C++ 17] is_abstract_v helper", "[utility]")
{
	REQUIRE_FALSE(is_abstract_v<test::O>);
	REQUIRE_FALSE(is_abstract_v<test::S>);

	REQUIRE(is_abstract_v<test::AB>);
	REQUIRE(is_abstract_v<test::AC>);
}

using substrate::is_polymorphic_v;
TEST_CASE("[C++ 17] is_polymorphic_v helper", "[utility]")
{
	REQUIRE_FALSE(is_polymorphic_v<test::S>);

	REQUIRE(is_polymorphic_v<test::O>);
	REQUIRE(is_polymorphic_v<test::AD>);
}

using substrate::is_empty_v;
TEST_CASE("[C++ 17] is_empty_v helper", "[utility]")
{
	REQUIRE_FALSE(is_empty_v<test::S>);
	REQUIRE_FALSE(is_empty_v<test::B>);
	REQUIRE_FALSE(is_empty_v<test::AF>);
	REQUIRE_FALSE(is_empty_v<test::AG>);

	REQUIRE(is_empty_v<test::A>);
	REQUIRE(is_empty_v<test::AE>);
	REQUIRE(is_empty_v<test::SU_is_empty_v>);
}

using substrate::is_pod_v;
TEST_CASE("[C++ 17] is_pod_v helper", "[utility]")
{
	REQUIRE_FALSE(is_pod_v<test::O>);
	REQUIRE_FALSE(is_pod_v<test::Z>);

	REQUIRE(is_pod_v<test::S>);
}

using substrate::is_standard_layout_v;
TEST_CASE("[C++ 17] is_standard_layout_v helper", "[utility]")
{
	REQUIRE_FALSE(is_standard_layout_v<test::O>);
	REQUIRE_FALSE(is_standard_layout_v<test::Z>);

	REQUIRE(is_standard_layout_v<test::S>);
}

using substrate::is_trivially_copyable_v;
TEST_CASE("[C++ 17] is_trivially_copyable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_copyable_v<test::O>);
	REQUIRE_FALSE(is_trivially_copyable_v<test::Q>);

	REQUIRE(is_trivially_copyable_v<test::S>);
	REQUIRE(is_trivially_copyable_v<test::AH>);
}

using substrate::is_trivial_v;
TEST_CASE("[C++ 17] is_trivial_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivial_v<test::Q>);

	REQUIRE(is_trivial_v<test::S>);
}

using substrate::is_volatile_v;
TEST_CASE("[C++ 17] is_volatile_v helper", "[utility]")
{
	REQUIRE_FALSE(is_volatile_v<test::A>);
	REQUIRE_FALSE(is_volatile_v<int>);

	REQUIRE(is_volatile_v<volatile int>);
}

using substrate::is_const_v;
TEST_CASE("[C++ 17] is_const_v helper", "[utility]")
{
	REQUIRE_FALSE(is_const_v<test::A>);
	REQUIRE_FALSE(is_const_v<int>);
	REQUIRE_FALSE(is_const_v<const int *>);
	REQUIRE_FALSE(is_const_v<const int &>);

	REQUIRE(is_const_v<const int>);
	REQUIRE(is_const_v<int* const>);
	REQUIRE(is_const_v<substrate::remove_reference_t<const int &>>);
}

using substrate::is_literal_type_v;
TEST_CASE("[C++ 17] is_literal_type_v helper", "[utility]")
{
	REQUIRE_FALSE(is_literal_type_v<test::AG>);

	REQUIRE(is_literal_type_v<test::S>);
}

#elif defined(SUBSTRATE_CXX11_COMPAT)

using substrate::is_same_v;
TEST_CASE("[C++ 17] is_same_v helper", "[utility]")
{
	using uint = unsigned int;
	using sint = signed int;

	REQUIRE_FALSE(is_same_v<int, bool>());
	REQUIRE_FALSE(is_same_v<float, int>());
	REQUIRE_FALSE(is_same_v<test::A, int>());
	REQUIRE_FALSE(is_same_v<uint, sint>());

	REQUIRE(is_same_v<test::A, test::A>());
	REQUIRE(is_same_v<uint, unsigned int>());
	REQUIRE(is_same_v<sint, signed int>());
}

using substrate::is_null_pointer_v;
TEST_CASE("[C++ 17] is_null_pointer_v helper", "[utility]")
{
	REQUIRE(is_null_pointer_v<decltype(nullptr)>());
	REQUIRE_FALSE(is_null_pointer_v<int *>());
}

using substrate::is_void_v;
TEST_CASE("[C++ 17] is_void_v helper", "[utility]")
{
	REQUIRE(is_void_v<void>());
	REQUIRE_FALSE(is_void_v<int>());
}

using substrate::is_integral_v;
TEST_CASE("[C++ 17] is_integral_v helper", "[utility]")
{
	REQUIRE_FALSE(is_integral_v<test::A>());
	REQUIRE_FALSE(is_integral_v<test::B>());
	REQUIRE_FALSE(is_integral_v<float>());
	REQUIRE_FALSE(is_integral_v<double>());

	REQUIRE(is_integral_v<int>());
	REQUIRE(is_integral_v<bool>());
	REQUIRE(is_integral_v<unsigned int>());
	REQUIRE(is_integral_v<unsigned char>());

}

using substrate::is_floating_point_v;
TEST_CASE("[C++ 17] is_floating_point_v helper", "[utility]")
{
	REQUIRE_FALSE(is_floating_point_v<test::A>());
	REQUIRE_FALSE(is_floating_point_v<test::B>());
	REQUIRE_FALSE(is_floating_point_v<int>());
	REQUIRE_FALSE(is_floating_point_v<bool>());
	REQUIRE_FALSE(is_floating_point_v<unsigned int>());
	REQUIRE_FALSE(is_floating_point_v<unsigned char>());

	REQUIRE(is_floating_point_v<float>());
	REQUIRE(is_floating_point_v<double>());
}

using substrate::is_array_v;
TEST_CASE("[C++ 17] is_array_v helper", "[utility]")
{
	REQUIRE_FALSE(is_array_v<test::A>());
	REQUIRE_FALSE(is_array_v<test::B>());
	REQUIRE_FALSE(is_array_v<int>());
	REQUIRE_FALSE(is_array_v<bool>());

	REQUIRE(is_array_v<test::A []>());
	REQUIRE(is_array_v<test::A [2]>());
	REQUIRE(is_array_v<test::B []>());
	REQUIRE(is_array_v<test::B [4]>());
	REQUIRE(is_array_v<int []>());
	REQUIRE(is_array_v<int [8]>());
	REQUIRE(is_array_v<bool []>());
	REQUIRE(is_array_v<bool [16]>());
}

using substrate::is_enum_v;
TEST_CASE("[C++ 17] is_enum_v helper", "[utility]")
{
	REQUIRE_FALSE(is_enum_v<test::A>());
	REQUIRE_FALSE(is_enum_v<int>());
	REQUIRE_FALSE(is_enum_v<bool>());

	REQUIRE(is_enum_v<test::B>());
	REQUIRE(is_enum_v<test::C>());

}

using substrate::is_union_v;
TEST_CASE("[C++ 17] is_union_v helper", "[utility]")
{
	REQUIRE_FALSE(is_union_v<test::A>());
	REQUIRE_FALSE(is_union_v<test::B>());
	REQUIRE_FALSE(is_union_v<test::E>());
	REQUIRE_FALSE(is_union_v<int>());
	REQUIRE_FALSE(is_union_v<void>());

	REQUIRE(is_union_v<test::D>());
}

using substrate::is_class_v;
TEST_CASE("[C++ 17] is_class_v helper", "[utility]")
{
	REQUIRE_FALSE(is_class_v<test::B>());
	REQUIRE_FALSE(is_class_v<test::D>());
	REQUIRE_FALSE(is_class_v<test::G>());
	REQUIRE_FALSE(is_class_v<int>());
	REQUIRE_FALSE(is_class_v<void>());

	REQUIRE(is_class_v<test::A>());
	REQUIRE(is_class_v<test::E>());
	REQUIRE(is_class_v<test::F>());
}

using substrate::is_function_v;
TEST_CASE("[C++ 17] is_function_v helper", "[utility]")
{
	REQUIRE_FALSE(is_function_v<test::A>());
	REQUIRE_FALSE(is_function_v<test::B>());
	REQUIRE_FALSE(is_function_v<test::H>());
	REQUIRE_FALSE(is_function_v<int>());
	REQUIRE_FALSE(is_function_v<void>());

	REQUIRE(is_function_v<int(void)>());
	REQUIRE(is_function_v<void(int)>());
	REQUIRE(is_function_v<decltype(test::bar)>());
	REQUIRE(is_function_v<test::SU_traits<decltype(&test::H::foo)>::member_type>());
}

using substrate::is_pointer_v;
TEST_CASE("[C++ 17] is_pointer_v helper", "[utility]")
{
	REQUIRE_FALSE(is_pointer_v<test::A>());
	REQUIRE_FALSE(is_pointer_v<test::B &>());
	REQUIRE_FALSE(is_pointer_v<int>());
	REQUIRE_FALSE(is_pointer_v<void>());
	REQUIRE_FALSE(is_pointer_v<char []>());
	REQUIRE_FALSE(is_pointer_v<bool [1]>());
	REQUIRE_FALSE(is_pointer_v<std::nullptr_t>());

	REQUIRE(is_pointer_v<test::A *>());
	REQUIRE(is_pointer_v<int *>());
	REQUIRE(is_pointer_v<void **>());
}

using substrate::is_lvalue_reference_v;
TEST_CASE("[C++ 17] is_lvalue_reference_v helper", "[utility]")
{
	REQUIRE_FALSE(is_lvalue_reference_v<test::A>());
	REQUIRE_FALSE(is_lvalue_reference_v<test::A &&>());
	REQUIRE_FALSE(is_lvalue_reference_v<int>());
	REQUIRE_FALSE(is_lvalue_reference_v<int &&>());

	REQUIRE(is_lvalue_reference_v<test::A &>());
	REQUIRE(is_lvalue_reference_v<int &>());
}

using substrate::is_rvalue_reference_v;
TEST_CASE("[C++ 17] is_rvalue_reference_v helper", "[utility]")
{
	REQUIRE_FALSE(is_rvalue_reference_v<test::A>());
	REQUIRE_FALSE(is_rvalue_reference_v<test::A &>());
	REQUIRE_FALSE(is_rvalue_reference_v<int>());
	REQUIRE_FALSE(is_rvalue_reference_v<int &>());

	REQUIRE(is_rvalue_reference_v<test::A &&>());
	REQUIRE(is_rvalue_reference_v<int &&>());
}

using substrate::is_member_object_pointer_v;
TEST_CASE("[C++ 17] is_member_object_pointer_v helper", "[utility]")
{
	REQUIRE_FALSE(is_member_object_pointer_v<int(test::A::*)()>());

	REQUIRE(is_member_object_pointer_v<int(test::A::*)>());
}

using substrate::is_member_function_pointer_v;
TEST_CASE("[C++ 17] is_member_function_pointer_v helper", "[utility]")
{
	REQUIRE_FALSE(is_member_function_pointer_v<decltype(test::bar)>());

	REQUIRE(is_member_function_pointer_v<decltype(&test::AI::foo)>());
}

using substrate::is_fundamental_v;
TEST_CASE("[C++ 17] is_fundamental_v helper", "[utility]")
{
	REQUIRE_FALSE(is_fundamental_v<test::A>());
	REQUIRE_FALSE(is_fundamental_v<test::A *>());
	REQUIRE_FALSE(is_fundamental_v<test::A &>());
	REQUIRE_FALSE(is_fundamental_v<test::A &&>());
	REQUIRE_FALSE(is_fundamental_v<int *>());
	REQUIRE_FALSE(is_fundamental_v<int &>());
	REQUIRE_FALSE(is_fundamental_v<int &&>());
	REQUIRE_FALSE(is_fundamental_v<decltype(test::bar)>());

	REQUIRE(is_fundamental_v<int>());
	REQUIRE(is_fundamental_v<float>());
	REQUIRE(is_fundamental_v<bool>());
}

using substrate::is_arithmetic_v;
TEST_CASE("[C++ 17] is_arithmetic_v helper", "[utility]")
{
	REQUIRE_FALSE(is_arithmetic_v<test::A>());
	REQUIRE_FALSE(is_arithmetic_v<test::B>());
	REQUIRE_FALSE(is_arithmetic_v<int *>());
	REQUIRE_FALSE(is_arithmetic_v<int &>());
	REQUIRE_FALSE(is_arithmetic_v<float *>());
	REQUIRE_FALSE(is_arithmetic_v<float &>());

	REQUIRE(is_arithmetic_v<int>());
	REQUIRE(is_arithmetic_v<int const>());
	REQUIRE(is_arithmetic_v<float>());
	REQUIRE(is_arithmetic_v<float const>());
	REQUIRE(is_arithmetic_v<char>());
	REQUIRE(is_arithmetic_v<char const>());
	REQUIRE(is_arithmetic_v<bool>());
	REQUIRE(is_arithmetic_v<bool const>());
}

using substrate::is_object_v;
TEST_CASE("[C++ 17] is_object_v helper", "[utility]")
{
	REQUIRE_FALSE(is_object_v<test::A &>());
	REQUIRE_FALSE(is_object_v<test::A &&>());
	REQUIRE_FALSE(is_object_v<test::B &>());
	REQUIRE_FALSE(is_object_v<test::B &&>());
	REQUIRE_FALSE(is_object_v<int &>());
	REQUIRE_FALSE(is_object_v<int &&>());

	REQUIRE(is_object_v<test::A>());
	REQUIRE(is_object_v<test::B>());
	REQUIRE(is_object_v<int>());
	REQUIRE(is_object_v<float>());
}

using substrate::is_compound_v;
TEST_CASE("[C++ 17] is_compound_v helper", "[utility]")
{
	REQUIRE_FALSE(is_compound_v<int>());

	REQUIRE(is_compound_v<test::A>());
}

using substrate::is_reference_v;
TEST_CASE("[C++ 17] is_reference_v helper", "[utility]")
{
	REQUIRE_FALSE(is_reference_v<test::A>());
	REQUIRE_FALSE(is_reference_v<int>());

	REQUIRE(is_reference_v<test::A &>());
	REQUIRE(is_reference_v<test::A &&>());
	REQUIRE(is_reference_v<int &>());
	REQUIRE(is_reference_v<int &&>());
}

using substrate::is_member_pointer_v;
TEST_CASE("[C++ 17] is_member_pointer_v helper", "[utility]")
{
	REQUIRE_FALSE(is_member_pointer_v<int>());

	REQUIRE(is_member_pointer_v<int(test::A::*)>());
}

using substrate::is_scalar_v;
TEST_CASE("[C++ 17] is_scalar_v helper", "[utility]")
{
	REQUIRE_FALSE(is_scalar_v<test::A>());

	REQUIRE(is_scalar_v<test::B>());
	REQUIRE(is_scalar_v<int>());
	REQUIRE(is_scalar_v<float>());
	REQUIRE(is_scalar_v<bool>());
	REQUIRE(is_scalar_v<char>());
}

using substrate::is_base_of_v;
TEST_CASE("[C++ 17] is_base_of_v helper", "[utility]")
{
	REQUIRE_FALSE(is_base_of_v<test::K, test::A>());
	REQUIRE_FALSE(is_base_of_v<test::I, test::A>());
	REQUIRE_FALSE(is_base_of_v<test::J, test::A>());
	REQUIRE_FALSE(is_base_of_v<test::J ,test::I>());

	REQUIRE(is_base_of_v<test::A, test::I>());
	REQUIRE(is_base_of_v<test::A, test::J>());
	REQUIRE(is_base_of_v<test::I, test::J>());
}

using substrate::is_convertible_v;
TEST_CASE("[C++ 17] is_convertible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_convertible_v<test::A, test::L>());
	REQUIRE_FALSE(is_convertible_v<test::L, test::M>());

	REQUIRE(is_convertible_v<test::L, test::A>());
}

using substrate::has_virtual_destructor_v;
TEST_CASE("[C++ 17] has_virtual_destructor_v helper", "[utility]")
{
	REQUIRE_FALSE(has_virtual_destructor_v<test::P>());

	REQUIRE(has_virtual_destructor_v<test::N>());
	REQUIRE(has_virtual_destructor_v<test::O>());
}

using substrate::is_destructible_v;
TEST_CASE("[C++ 17] is_destructible_v helper", "[utility]")
{
	REQUIRE(is_destructible_v<test::P>());
	REQUIRE(is_destructible_v<test::Q>());
}

using substrate::is_trivially_destructible_v;
TEST_CASE("[C++ 17] is_trivially_destructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_destructible_v<test::Q>());

	REQUIRE(is_trivially_destructible_v<test::P>());
}

using substrate::is_nothrow_destructible_v;
TEST_CASE("[C++ 17] is_nothrow_destructible_v helper", "[utility]")
{
	REQUIRE(is_nothrow_destructible_v<test::P>());
	REQUIRE(is_nothrow_destructible_v<test::Q>());
}

using substrate::is_move_assignable_v;
TEST_CASE("[C++ 17] is_move_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_move_assignable_v<int []>());
	REQUIRE_FALSE(is_move_assignable_v<int [4]>());

	REQUIRE(is_move_assignable_v<test::S>());
	REQUIRE(is_move_assignable_v<test::R>());
	REQUIRE(is_move_assignable_v<int>());
}

using substrate::is_trivially_move_assignable_v;
TEST_CASE("[C++ 17] is_trivially_move_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_move_assignable_v<test::R>());
	REQUIRE_FALSE(is_trivially_move_assignable_v<int []>());
	REQUIRE_FALSE(is_trivially_move_assignable_v<int [4]>());

	REQUIRE(is_trivially_move_assignable_v<test::S>());
	REQUIRE(is_trivially_move_assignable_v<int>());
}

using substrate::is_nothrow_move_assignable_v;
TEST_CASE("[C++ 17] is_nothrow_move_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_move_assignable_v<test::R>());
	REQUIRE_FALSE(is_nothrow_move_assignable_v<int []>());
	REQUIRE_FALSE(is_nothrow_move_assignable_v<int [4]>());

	REQUIRE(is_nothrow_move_assignable_v<test::S>());
	REQUIRE(is_nothrow_move_assignable_v<int>());
}

using substrate::is_copy_assignable_v;
TEST_CASE("[C++ 17] is_copy_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_copy_assignable_v<int []>());
	REQUIRE_FALSE(is_copy_assignable_v<int [4]>());

	REQUIRE(is_copy_assignable_v<test::S>());
	REQUIRE(is_copy_assignable_v<int>());
}

using substrate::is_trivially_copy_assignable_v;
TEST_CASE("[C++ 17] is_trivially_copy_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_copy_assignable_v<int []>());
	REQUIRE_FALSE(is_trivially_copy_assignable_v<int [4]>());

	REQUIRE(is_trivially_copy_assignable_v<test::S>());
	REQUIRE(is_trivially_copy_assignable_v<int>());
}

using substrate::is_nothrow_copy_assignable_v;
TEST_CASE("[C++ 17] is_nothrow_copy_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_copy_assignable_v<int []>());
	REQUIRE_FALSE(is_nothrow_copy_assignable_v<int [4]>());

	REQUIRE(is_nothrow_copy_assignable_v<test::S>());
	REQUIRE(is_nothrow_copy_assignable_v<int>());
}

using substrate::is_assignable_v;
TEST_CASE("[C++ 17] is_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_assignable_v<test::S, int>());
	REQUIRE_FALSE(is_assignable_v<test::V, int>());
	REQUIRE_FALSE(is_assignable_v<int, int>());
	REQUIRE_FALSE(is_assignable_v<int, int &>());
	REQUIRE_FALSE(is_assignable_v<int, float>());

	REQUIRE(is_assignable_v<test::S &, test::S &>());
	REQUIRE(is_assignable_v<test::U, int>());
	REQUIRE(is_assignable_v<int &, int>());
	REQUIRE(is_assignable_v<int &, int &>());
	REQUIRE(is_assignable_v<int &, float>());
}

using substrate::is_trivially_assignable_v;
TEST_CASE("[C++ 17] is_trivially_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_assignable_v<test::S, int>());
	REQUIRE_FALSE(is_trivially_assignable_v<test::U, int>());
	REQUIRE_FALSE(is_trivially_assignable_v<test::V, int>());
	REQUIRE_FALSE(is_trivially_assignable_v<int, int>());
	REQUIRE_FALSE(is_trivially_assignable_v<int, int &>());
	REQUIRE_FALSE(is_trivially_assignable_v<int, float>());

	REQUIRE(is_trivially_assignable_v<test::S &, test::S &>());
	REQUIRE(is_trivially_assignable_v<int &, int>());
	REQUIRE(is_trivially_assignable_v<int &, int &>());
	REQUIRE(is_trivially_assignable_v<int &, float>());
}

using substrate::is_nothrow_assignable_v;
TEST_CASE("[C++ 17] is_nothrow_assignable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_assignable_v<test::S, int>());
	REQUIRE_FALSE(is_nothrow_assignable_v<test::U, int>());
	REQUIRE_FALSE(is_nothrow_assignable_v<test::V, int>());
	REQUIRE_FALSE(is_nothrow_assignable_v<int, int>());
	REQUIRE_FALSE(is_nothrow_assignable_v<int, int &>());
	REQUIRE_FALSE(is_nothrow_assignable_v<int, float>());

	REQUIRE(is_nothrow_assignable_v<test::S &, test::S &>());
	REQUIRE(is_nothrow_assignable_v<int &, int>());
	REQUIRE(is_nothrow_assignable_v<int &, int &>());
	REQUIRE(is_nothrow_assignable_v<int &, float>());
}

using substrate::is_move_constructible_v;
TEST_CASE("[C++ 17] is_move_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_move_constructible_v<test::N>());

	REQUIRE(is_move_constructible_v<test::S>());
	REQUIRE(is_move_constructible_v<test::T>());
	REQUIRE(is_move_constructible_v<test::W>());
	REQUIRE(is_move_constructible_v<test::X>());
}

using substrate::is_trivially_move_constructible_v;
TEST_CASE("[C++ 17] is_trivially_move_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_move_constructible_v<test::N>());
	REQUIRE_FALSE(is_trivially_move_constructible_v<test::T>());
	REQUIRE_FALSE(is_trivially_move_constructible_v<test::W>());
	REQUIRE_FALSE(is_trivially_move_constructible_v<test::X>());

	REQUIRE(is_trivially_move_constructible_v<test::S>());
}

using substrate::is_nothrow_move_constructible_v;
TEST_CASE("[C++ 17] is_nothrow_move_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_move_constructible_v<test::N>());
	REQUIRE_FALSE(is_nothrow_move_constructible_v<test::W>());
	REQUIRE_FALSE(is_nothrow_move_constructible_v<test::X>());

	REQUIRE(is_nothrow_move_constructible_v<test::S>());
	REQUIRE(is_nothrow_move_constructible_v<test::T>());
}

using substrate::is_copy_constructible_v;
TEST_CASE("[C++ 17] is_copy_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_copy_constructible_v<test::N>());

	REQUIRE(is_copy_constructible_v<test::S>());
	REQUIRE(is_copy_constructible_v<test::T>());
	REQUIRE(is_copy_constructible_v<test::W>());
	REQUIRE(is_copy_constructible_v<test::X>());
}

using substrate::is_trivially_copy_constructible_v;
TEST_CASE("[C++ 17] is_trivially_copy_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_copy_constructible_v<test::N>());
	REQUIRE_FALSE(is_trivially_copy_constructible_v<test::T>());
	REQUIRE_FALSE(is_trivially_copy_constructible_v<test::W>());
	REQUIRE_FALSE(is_trivially_copy_constructible_v<test::X>());

	REQUIRE(is_trivially_copy_constructible_v<test::S>());
}

using substrate::is_nothrow_copy_constructible_v;
TEST_CASE("[C++ 17] is_nothrow_copy_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<test::N>());
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<test::T>());
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<test::W>());
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<test::X>());

	REQUIRE(is_nothrow_copy_constructible_v<test::S>());
}

using substrate::is_default_constructible_v;
TEST_CASE("[C++ 17] is_default_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_default_constructible_v<test::N>());
	REQUIRE_FALSE(is_default_constructible_v<test::U>());
	REQUIRE_FALSE(is_default_constructible_v<test::V>());
	REQUIRE_FALSE(is_default_constructible_v<test::AJ>());

	REQUIRE(is_default_constructible_v<test::S>());
	REQUIRE(is_default_constructible_v<test::T>());
}

using substrate::is_trivially_default_constructible_v;
TEST_CASE("[C++ 17] is_trivially_default_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_default_constructible_v<test::N>());
	REQUIRE_FALSE(is_trivially_default_constructible_v<test::T>());
	REQUIRE_FALSE(is_trivially_default_constructible_v<test::U>());
	REQUIRE_FALSE(is_trivially_default_constructible_v<test::V>());
	REQUIRE_FALSE(is_trivially_default_constructible_v<test::AJ>());

	REQUIRE(is_trivially_default_constructible_v<test::S>());
}

using substrate::is_nothrow_default_constructible_v;
TEST_CASE("[C++ 17] is_nothrow_default_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_default_constructible_v<test::N>());
	REQUIRE_FALSE(is_nothrow_default_constructible_v<test::U>());
	REQUIRE_FALSE(is_nothrow_default_constructible_v<test::V>());
	REQUIRE_FALSE(is_nothrow_default_constructible_v<test::AJ>());

	REQUIRE(is_nothrow_default_constructible_v<test::S>());
	REQUIRE(is_nothrow_default_constructible_v<test::T>());
}

using substrate::is_constructible_v;
TEST_CASE("[C++ 17] is_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_constructible_v<test::Y, test::A>());

	REQUIRE(is_constructible_v<test::Y, int>());
	REQUIRE(is_constructible_v<test::Y, int, double>());
}

using substrate::is_trivially_constructible_v;
TEST_CASE("[C++ 17] is_trivially_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_constructible_v<test::Y, int>());

	REQUIRE(is_trivially_constructible_v<test::Y, const test::Y &>());
}

using substrate::is_nothrow_constructible_v;
TEST_CASE("[C++ 17] is_nothrow_constructible_v helper", "[utility]")
{
	REQUIRE_FALSE(is_nothrow_constructible_v<test::Y, int>());

	REQUIRE(is_nothrow_constructible_v<test::Y, int, double>());
}

using substrate::is_unsigned_v;
TEST_CASE("[C++ 17] is_unsigned_v helper", "[utility]")
{
	enum D : unsigned int {};
	enum struct E : unsigned int {};

	REQUIRE_FALSE(is_unsigned_v<test::A>());
	REQUIRE_FALSE(is_unsigned_v<test::B>());
	REQUIRE_FALSE(is_unsigned_v<test::C>());
	REQUIRE_FALSE(is_unsigned_v<D>());
	REQUIRE_FALSE(is_unsigned_v<E>());
	REQUIRE_FALSE(is_unsigned_v<int>());
	REQUIRE_FALSE(is_unsigned_v<signed int>());

	REQUIRE(is_unsigned_v<unsigned int>());
}

using substrate::is_signed_v;
TEST_CASE("[C++ 17] is_signed_v helper", "[utility]")
{
	enum D : unsigned int {};
	enum struct E : unsigned int {};

	REQUIRE_FALSE(is_signed_v<test::A>());
	REQUIRE_FALSE(is_signed_v<test::B>());
	REQUIRE_FALSE(is_signed_v<test::C>());
	REQUIRE_FALSE(is_signed_v<D>());
	REQUIRE_FALSE(is_signed_v<E>());
	REQUIRE_FALSE(is_signed_v<unsigned int>());

	REQUIRE(is_signed_v<int>());
	REQUIRE(is_signed_v<signed int>());
}

using substrate::is_abstract_v;
TEST_CASE("[C++ 17] is_abstract_v helper", "[utility]")
{
	REQUIRE_FALSE(is_abstract_v<test::S>());
	REQUIRE_FALSE(is_abstract_v<test::O>());

	REQUIRE(is_abstract_v<test::AB>());
	REQUIRE(is_abstract_v<test::AC>());
}

using substrate::is_polymorphic_v;
TEST_CASE("[C++ 17] is_polymorphic_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { virtual void bar(); virtual ~B() = default; };
	struct C : B {};

	REQUIRE_FALSE(is_polymorphic_v<A>());

	REQUIRE(is_polymorphic_v<B>());
	REQUIRE(is_polymorphic_v<C>());
}

struct SU_is_empty_v_C { static int bar; };
using substrate::is_empty_v;
TEST_CASE("[C++ 17] is_empty_v helper", "[utility]")
{
	struct A {};
	struct B { int foo; };
	struct D { virtual ~D(); };

	enum E : int {};
	union F {};

	REQUIRE_FALSE(is_empty_v<B>());
	REQUIRE_FALSE(is_empty_v<D>());
	REQUIRE_FALSE(is_empty_v<E>());
	REQUIRE_FALSE(is_empty_v<F>());

	REQUIRE(is_empty_v<A>());
	REQUIRE(is_empty_v<SU_is_empty_v_C>());
}

using substrate::is_pod_v;
TEST_CASE("[C++ 17] is_pod_v helper", "[utility]")
{
	REQUIRE_FALSE(is_pod_v<test::O>());
	REQUIRE_FALSE(is_pod_v<test::Z>());

	REQUIRE(is_pod_v<test::S>());
}

using substrate::is_standard_layout_v;
TEST_CASE("[C++ 17] is_standard_layout_v helper", "[utility]")
{
	REQUIRE_FALSE(is_standard_layout_v<test::O>());
	REQUIRE_FALSE(is_standard_layout_v<test::Z>());

	REQUIRE(is_standard_layout_v<test::S>());
}

using substrate::is_trivially_copyable_v;
TEST_CASE("[C++ 17] is_trivially_copyable_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivially_copyable_v<test::O>());
	REQUIRE_FALSE(is_trivially_copyable_v<test::T>());

	REQUIRE(is_trivially_copyable_v<test::S>());
	REQUIRE(is_trivially_copyable_v<test::AA>());
}

using substrate::is_trivial_v;
TEST_CASE("[C++ 17] is_trivial_v helper", "[utility]")
{
	REQUIRE_FALSE(is_trivial_v<test::T>());

	REQUIRE(is_trivial_v<test::S>());
}

using substrate::is_volatile_v;
TEST_CASE("[C++ 17] is_volatile_v helper", "[utility]")
{
	REQUIRE_FALSE(is_volatile_v<test::A>());
	REQUIRE_FALSE(is_volatile_v<int>());

	REQUIRE(is_volatile_v<volatile int>());
}

using substrate::is_const_v;
TEST_CASE("[C++ 17] is_const_v helper", "[utility]")
{
	REQUIRE_FALSE(is_const_v<test::A>());
	REQUIRE_FALSE(is_const_v<int>());
	REQUIRE_FALSE(is_const_v<const int *>());
	REQUIRE_FALSE(is_const_v<const int &>());

	REQUIRE(is_const_v<const int>());
	REQUIRE(is_const_v<int *const>());
	REQUIRE(is_const_v<substrate::remove_reference_t<const int &>>());
}

using substrate::is_literal_type_v;
TEST_CASE("[C++ 17] is_literal_type_v helper", "[utility]")
{
	REQUIRE_FALSE(is_literal_type_v<test::T>());

	REQUIRE(is_literal_type_v<test::S>());
}

#endif

/* C++ 14 helpers */
#if __cplusplus < 201402L

using substrate::enable_if_t;
TEST_CASE("[C++ 14] enable_if_t helper", "[utility]") { }

using substrate::underlying_type_t;
TEST_CASE("[C++ 14] underlying_type_t helper", "[utility]")
{
	enum A : int {};
	enum B : long {};
	enum C : unsigned short{};
	enum D {};

	REQUIRE(std::is_same<int, underlying_type_t<A>>::value);
	REQUIRE(std::is_same<long, underlying_type_t<B>>::value);
	REQUIRE(std::is_same<unsigned short, underlying_type_t<C>>::value);
	REQUIRE(std::is_same<unsigned int, underlying_type_t<D>>::value);
}

using substrate::conditional_t;
TEST_CASE("[C++ 14] conditional_t helper", "[utility]") { }

using substrate::remove_reference_t;
TEST_CASE("[C++ 14] remove_reference_t helper", "[utility]")
{
	REQUIRE(std::is_same<int, remove_reference_t<int>>::value);
	REQUIRE(std::is_same<int, remove_reference_t<int &>>::value);
	REQUIRE(std::is_same<int, remove_reference_t<int &&>>::value);
}

using substrate::remove_cv_t;
TEST_CASE("[C++ 14] remove_cv_t helper", "[utility]")
{
	using A = const int;
	using B = const int *;
	using C = int *const;
	using D = volatile int;
	using E = volatile int *;
	using F = int *volatile;
	using G = const volatile int;
	using H = const volatile int *;
	using I = int *const volatile;

	REQUIRE(std::is_same<int, remove_cv_t<A>>::value);
	REQUIRE(std::is_same<B, remove_cv_t<B>>::value);
	REQUIRE(std::is_same<int *, remove_cv_t<C>>::value);
	REQUIRE(std::is_same<int, remove_cv_t<D>>::value);
	REQUIRE(std::is_same<E, remove_cv_t<E>>::value);
	REQUIRE(std::is_same<int *, remove_cv_t<F>>::value);
	REQUIRE(std::is_same<int, remove_cv_t<G>>::value);
	REQUIRE(std::is_same<H, remove_cv_t<H>>::value);
	REQUIRE(std::is_same<int *, remove_cv_t<I>>::value);
}

using substrate::remove_const_t;
TEST_CASE("[C++ 14] remove_const_t helper", "[utility]")
{
	using A = const int;
	using B = const int *;
	using C = int *const;
	using D = volatile int;
	using E = volatile int *;
	using F = int *volatile;
	using G = const volatile int;
	using H = const volatile int *;
	using I = int *const volatile;

	REQUIRE(std::is_same<int, remove_const_t<A>>::value);
	REQUIRE(std::is_same<B, remove_const_t<B>>::value);
	REQUIRE(std::is_same<int *, remove_const_t<C>>::value);
	REQUIRE(std::is_same<D, remove_const_t<D>>::value);
	REQUIRE(std::is_same<E, remove_const_t<E>>::value);
	REQUIRE(std::is_same<F, remove_const_t<F>>::value);
	REQUIRE(std::is_same<volatile int, remove_const_t<G>>::value);
	REQUIRE(std::is_same<H, remove_const_t<H>>::value);
	REQUIRE(std::is_same<int *volatile, remove_const_t<I>>::value);
}

using substrate::remove_volatile_t;
TEST_CASE("[C++ 14] remove_volatile_t helper", "[utility]")
{
	using A = const int;
	using B = const int *;
	using C = int *const;
	using D = volatile int;
	using E = volatile int *;
	using F = int *volatile;
	using G = const volatile int;
	using H = const volatile int *;
	using I = int *const volatile;

	REQUIRE(std::is_same<A, remove_volatile_t<A>>::value);
	REQUIRE(std::is_same<B, remove_volatile_t<B>>::value);
	REQUIRE(std::is_same<C, remove_volatile_t<C>>::value);
	REQUIRE(std::is_same<int, remove_volatile_t<D>>::value);
	REQUIRE(std::is_same<E, remove_volatile_t<E>>::value);
	REQUIRE(std::is_same<int *, remove_volatile_t<F>>::value);
	REQUIRE(std::is_same<const int, remove_volatile_t<G>>::value);
	REQUIRE(std::is_same<H, remove_volatile_t<H>>::value);
	REQUIRE(std::is_same<int *const, remove_volatile_t<I>>::value);
}

using  substrate::add_cv_t;
TEST_CASE("[C++14] add_cv_t helper", "[utility]")
{
	using A = int;
	using B = int *;
	using C = const int;
	using D = const int *;
	using E = int *const;
	using F = volatile int;
	using G = volatile int *;
	using H = int *volatile;

	using I = int *const volatile;
	using J = const volatile int;

	REQUIRE(std::is_same<J, add_cv_t<A>>::value);
	REQUIRE(std::is_same<I, add_cv_t<B>>::value);
	REQUIRE(std::is_same<J, add_cv_t<C>>::value);
	REQUIRE(std::is_same<int const *const volatile, add_cv_t<D>>::value);
	REQUIRE(std::is_same<I, add_cv_t<E>>::value);
	REQUIRE(std::is_same<J, add_cv_t<F>>::value);
	REQUIRE(std::is_same<int volatile *const volatile, add_cv_t<G>>::value);
	REQUIRE(std::is_same<I, add_cv_t<H>>::value);
}

using  substrate::add_const_t;
TEST_CASE("[C++14] add_const_t helper", "[utility]")
{
	using A = int;
	using B = int *;
	using C = const int;
	using D = const int *;
	using E = int *const;
	using F = volatile int;
	using G = volatile int *;
	using H = int *volatile;

	using I = int *const volatile;
	using J = const volatile int;

	REQUIRE(std::is_same<int const, add_const_t<A>>::value);
	REQUIRE(std::is_same<int * const, add_const_t<B>>::value);
	REQUIRE(std::is_same<int const, add_const_t<C>>::value);
	REQUIRE(std::is_same<int const *const, add_const_t<D>>::value);
	REQUIRE(std::is_same<int *const, add_const_t<E>>::value);
	REQUIRE(std::is_same<J, add_const_t<F>>::value);
	REQUIRE(std::is_same<int volatile *const, add_const_t<G>>::value);
	REQUIRE(std::is_same<I, add_const_t<H>>::value);
}

using  substrate::add_volatile_t;
TEST_CASE("[C++14] add_volatile_t helper", "[utility]")
{
	using A = int;
	using B = int *;
	using C = const int;
	using D = const int *;
	using E = int *const;
	using F = volatile int;
	using G = volatile int *;
	using H = int *volatile;

	using I = int *const volatile;

	REQUIRE(std::is_same<int volatile, add_volatile_t<A>>::value);
	REQUIRE(std::is_same<int *volatile, add_volatile_t<B>>::value);
	REQUIRE(std::is_same<int const volatile, add_volatile_t<C>>::value);
	REQUIRE(std::is_same<int const *volatile, add_volatile_t<D>>::value);
	REQUIRE(std::is_same<I, add_volatile_t<E>>::value);
	REQUIRE(std::is_same<volatile int, add_volatile_t<F>>::value);
	REQUIRE(std::is_same<int volatile *volatile, add_volatile_t<G>>::value);
	REQUIRE(std::is_same<int *volatile, add_volatile_t<H>>::value);
}

using  substrate::add_lvalue_reference_t;
TEST_CASE("[C++14] add_lvalue_reference_t helper", "[utility]")
{
	REQUIRE(std::is_same<int &, add_lvalue_reference_t<int>>::value);
}

using  substrate::add_rvalue_reference_t;
TEST_CASE("[C++14] add_rvalue_reference_t helper", "[utility]")
{
	REQUIRE(std::is_same<int &&, add_rvalue_reference_t<int>>::value);
}

using  substrate::remove_pointer_t;
TEST_CASE("[C++14] remove_pointer_t helper", "[utility]")
{
	REQUIRE(std::is_same<int, remove_pointer_t<int *>>::value);
	REQUIRE(std::is_same<int *, remove_pointer_t<int **>>::value);
}

using  substrate::add_pointer_t;
TEST_CASE("[C++14] add_pointer_t helper", "[utility]")
{
	REQUIRE(std::is_same<int *, add_pointer_t<int>>::value);
	REQUIRE(std::is_same<int **, add_pointer_t<int *>>::value);
}

using  substrate::make_signed_t;
TEST_CASE("[C++14] make_signed_t helper", "[utility]")
{
	REQUIRE_FALSE(std::is_same<unsigned int, make_signed_t<int>>::value);
	REQUIRE(std::is_same<int, make_signed_t<int>>::value);
}

using  substrate::make_unsigned_t;
TEST_CASE("[C++14] make_unsigned_t helper", "[utility]")
{
	REQUIRE_FALSE(std::is_same<int, make_unsigned_t<int>>::value);
	REQUIRE(std::is_same<unsigned int, make_unsigned_t<int>>::value);
}
#endif


using substrate::decode_typename;
TEST_CASE("C++ typename decoding", "[utility]")
{
	using A = int;
	using Au = unsigned int;
	using B = int *;
	using Bu = unsigned int *;
	using C = const int;
	using Cu = const unsigned int;
	using D = const int *;
	using Du = const unsigned int *;
	using E = int *const;
	using Eu = unsigned int *const;
	using F = volatile int;
	using Fu = volatile unsigned int;
	using H = int *volatile;
	using Hu = unsigned int *volatile;
	using G = volatile int *;
	using Gu = volatile unsigned int *;
	using I = int *const volatile;
	using Iu = unsigned int *const volatile;
	using J = const volatile int;
	using Ju = const volatile unsigned int;

	REQUIRE(decode_typename<A>() == "int");
	REQUIRE(decode_typename<Au>() == "unsigned int");
	REQUIRE(decode_typename<B>() == "int*");
	REQUIRE(decode_typename<Bu>() == "unsigned int*");
	REQUIRE(decode_typename<C>() == "int const");
	REQUIRE(decode_typename<Cu>() == "unsigned int const");
	REQUIRE(decode_typename<D>() == "int const*");
	REQUIRE(decode_typename<Du>() == "unsigned int const*");
	REQUIRE(decode_typename<E>() == "int* const");
	REQUIRE(decode_typename<Eu>() == "unsigned int* const");
	REQUIRE(decode_typename<F>() == "int volatile");
	REQUIRE(decode_typename<Fu>() == "unsigned int volatile");
	REQUIRE(decode_typename<G>() == "int volatile*");
	REQUIRE(decode_typename<Gu>() == "unsigned int volatile*");
	REQUIRE(decode_typename<H>() == "int* volatile");
	REQUIRE(decode_typename<Hu>() == "unsigned int* volatile");
	REQUIRE(decode_typename<I>() == "int* const volatile");
	REQUIRE(decode_typename<Iu>() == "unsigned int* const volatile");
	REQUIRE(decode_typename<J>() == "int const volatile");
	REQUIRE(decode_typename<Ju>() == "unsigned int const volatile");
}

using substrate::shift_nibble;
TEST_CASE("shift nibble", "[utility]")
{
	constexpr uint64_t value{0x000000000000000FLLU};
	constexpr uint64_t value2{0x0123456789ABCDEFLLU};

	REQUIRE(shift_nibble<decltype(value)>(value, 1U) == 0x00000000000000F0LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 2U) == 0x0000000000000F00LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 3U) == 0x000000000000F000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 4U) == 0x00000000000F0000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 5U) == 0x0000000000F00000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 6U) == 0x000000000F000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 7U) == 0x00000000F0000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 8U) == 0x0000000F00000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 9U) == 0x000000F000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 10U) == 0x00000F0000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 11U) == 0x0000F00000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 12U) == 0x000F000000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 13U) == 0x00F0000000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 14U) == 0x0F00000000000000LLU);
	REQUIRE(shift_nibble<decltype(value)>(value, 15U) == 0xF000000000000000LLU);

	REQUIRE(shift_nibble<decltype(value2)>(value2, 2U) == 0x23456789ABCDEF01LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 4U) == 0x456789ABCDEF0123LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 6U) == 0x6789ABCDEF012345LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 8U) == 0x89ABCDEF01234567LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 10U) == 0xABCDEF0123456789LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 12U) == 0xCDEF0123456789ABLLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 14U) == 0xEF0123456789ABCDLLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 16U) == 0x0123456789ABCDEFLLU);

	REQUIRE(shift_nibble<decltype(value2)>(value2, 4U) == 0x456789ABCDEF0123LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 8U) == 0x89ABCDEF01234567LLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 12U) == 0xCDEF0123456789ABLLU);
	REQUIRE(shift_nibble<decltype(value2)>(value2, 16U) == 0x0123456789ABCDEFLLU);
}

using substrate::swap16;
using substrate::swap32;
using substrate::swap64;
TEST_CASE("bswap tests", "[utility]")
{
	SECTION("bswap16")
	{
		REQUIRE(swap16(0x3412U) == 0x1234U);
		REQUIRE(swap16(0x1234U) == 0x3412U);
		REQUIRE(swap16(0x2413U) == 0x1324U);
		REQUIRE(swap16(0x2143U) == 0x4321U);
	}

	SECTION("bswap32")
	{
		REQUIRE(swap32(0x78563412U) == 0x12345678U);
		REQUIRE(swap32(0x12345678U) == 0x78563412U);
		REQUIRE(swap32(0x34127856U) == 0x56781234U);
		REQUIRE(swap32(0x56341278U) == 0x78123456U);
	}

	SECTION("bswap64")
	{
		REQUIRE(swap64(0xEFCDAB8967452301LLU) == 0x0123456789ABCDEFLLU);
		REQUIRE(swap64(0x0123456789ABCDEFLLU) == 0xEFCDAB8967452301LLU);
		REQUIRE(swap64(0x89ABCDEF01234567LLU) == 0x67452301EFCDAB89LLU);
		REQUIRE(swap64(0xAB8967452301EFCDLLU) == 0xCDEF0123456789ABLLU);
	}
}

using substrate::leb128_decode;
using substrate::leb128_encode;
TEST_CASE("leb128 tests", "[utility]")
{
	std::random_device rand;
	const size_t check_itr = 8192;

	SECTION("signed leb128 int8_t")
	{
		std::uniform_int_distribution<int8_t> int8Dist{std::numeric_limits<int8_t>::min()};
		for (size_t c{}; c < check_itr; ++c)
		{
			auto value = int8Dist(rand);
			auto enc = leb128_encode<int8_t>(value);
			auto res = leb128_decode<int8_t>(enc);
			REQUIRE(value == res);
		}
	}

	SECTION("signed leb128 int16_t")
	{
		std::uniform_int_distribution<int16_t> int16Dist{std::numeric_limits<int16_t>::min()};
		for (size_t c{}; c < check_itr; ++c)
		{
			auto value = int16Dist(rand);
			auto enc = leb128_encode<int16_t>(value);
			auto res = leb128_decode<int16_t>(enc);
			REQUIRE(value == res);
		}
	}

	SECTION("signed leb128 int32_t")
	{
		std::uniform_int_distribution<int32_t> int32Dist{std::numeric_limits<int32_t>::min()};
		for (size_t c{}; c < check_itr; ++c)
		{
			auto value = int32Dist(rand);
			auto enc = leb128_encode<int32_t>(value);
			auto res = leb128_decode<int32_t>(enc);
			REQUIRE(value == res);
		}
	}

	SECTION("signed leb128 int64_t")
	{
		std::uniform_int_distribution<int64_t> int64Dist{std::numeric_limits<int64_t>::min()};
		for (size_t c{}; c < check_itr; ++c)
		{
			auto value = int64Dist(rand);
			auto enc = leb128_encode<int64_t>(value);
			auto res = leb128_decode<int64_t>(enc);
			REQUIRE(value == res);
		}
	}

	SECTION("unsigned leb128 uint8_t")
	{
		std::uniform_int_distribution<uint8_t> uint8Dist{std::numeric_limits<uint8_t>::min()};
		for (size_t c{}; c < check_itr; ++c)
		{
			auto value = uint8Dist(rand);
			auto enc = leb128_encode<uint8_t>(value);
			auto res = leb128_decode<uint8_t>(enc);
			REQUIRE(value == res);
		}
	}

	SECTION("unsigned leb128 uint16_t")
	{
		std::uniform_int_distribution<uint16_t> uint16Dist{std::numeric_limits<uint16_t>::min()};
		for (size_t c{}; c < check_itr; ++c)
		{
			auto value = uint16Dist(rand);
			auto enc = leb128_encode<uint16_t>(value);
			auto res = leb128_decode<uint16_t>(enc);
			REQUIRE(value == res);
		}
	}

	SECTION("unsigned leb128 uint32_t")
	{
		std::uniform_int_distribution<uint32_t> uint32Dist{std::numeric_limits<uint32_t>::min()};
		for (size_t c{}; c < check_itr; ++c)
		{
			auto value = uint32Dist(rand);
			auto enc = leb128_encode<uint32_t>(value);
			auto res = leb128_decode<uint32_t>(enc);
			REQUIRE(value == res);
		}
	}

	SECTION("unsigned leb128 uint64_t")
	{
		std::uniform_int_distribution<uint64_t> uint64Dist{std::numeric_limits<uint64_t>::min()};
		for (size_t c{}; c < check_itr; ++c)
		{
			auto value = uint64Dist(rand);
			auto enc = leb128_encode<uint64_t>(value);
			auto res = leb128_decode<uint64_t>(enc);
			REQUIRE(value == res);
		}
	}
}

using substrate::truthy_t;
using substrate::falsy_t;
using substrate::has_nullable_ctor;

TEST_CASE("has_nullable_ctor", "[utility]")
{
	REQUIRE_FALSE(std::is_pod<test::AK>::value);
	REQUIRE(has_nullable_ctor<test::AK>::value);
}

/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
