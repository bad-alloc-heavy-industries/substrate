// SPDX-License-Identifier: BSD-3-Clause
#include <cstdint>
#include <string>
#include <random>

#if __cplusplus < 201402L && !defined(SUBSTRATE_CXX11_COMPAT)
#define SUBSTRATE_CXX11_COMPAT
#endif
#include <substrate/utility>
#include <catch.hpp>

/* C++ 17 helpers */
#if __cplusplus < 201703L && __cplusplus >= 201402L

using substrate::is_same_v;
TEST_CASE( "[C++ 17] is_same_v helper", "[utility]")
{
	class A {};

	using uint = unsigned int;
	using sint = signed int;

	REQUIRE_FALSE(is_same_v<int, bool>);
	REQUIRE_FALSE(is_same_v<float, int>);
	REQUIRE_FALSE(is_same_v<A, int>);
	REQUIRE_FALSE(is_same_v<uint, sint>);

	REQUIRE(is_same_v<A, A>);
	REQUIRE(is_same_v<uint, unsigned int>);
	REQUIRE(is_same_v<sint, signed int>);
}

using substrate::is_null_pointer_v;
TEST_CASE( "[C++ 17] is_null_pointer_v helper", "[utility]")
{
	REQUIRE(is_null_pointer_v<decltype(nullptr)>);
	REQUIRE_FALSE(is_null_pointer_v<int*>);
}

using substrate::is_void_v;
TEST_CASE( "[C++ 17] is_void_v helper", "[utility]")
{
	REQUIRE(is_void_v<void>);
	REQUIRE_FALSE(is_void_v<int>);
}

using substrate::is_integral_v;
TEST_CASE( "[C++ 17] is_integral_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_integral_v<A>);
	REQUIRE_FALSE(is_integral_v<B>);
	REQUIRE_FALSE(is_integral_v<float>);
	REQUIRE_FALSE(is_integral_v<double>);

	REQUIRE(is_integral_v<int>);
	REQUIRE(is_integral_v<bool>);
	REQUIRE(is_integral_v<unsigned int>);
	REQUIRE(is_integral_v<unsigned char>);

}

using substrate::is_floating_point_v;
TEST_CASE( "[C++ 17] is_floating_point_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_floating_point_v<A>);
	REQUIRE_FALSE(is_floating_point_v<B>);
	REQUIRE_FALSE(is_floating_point_v<int>);
	REQUIRE_FALSE(is_floating_point_v<bool>);
	REQUIRE_FALSE(is_floating_point_v<unsigned int>);
	REQUIRE_FALSE(is_floating_point_v<unsigned char>);

	REQUIRE(is_floating_point_v<float>);
	REQUIRE(is_floating_point_v<double>);
}

using substrate::is_array_v;
TEST_CASE( "[C++ 17] is_array_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_array_v<A>);
	REQUIRE_FALSE(is_array_v<B>);
	REQUIRE_FALSE(is_array_v<int>);
	REQUIRE_FALSE(is_array_v<bool>);

	REQUIRE(is_array_v<A[]>);
	REQUIRE(is_array_v<A[2]>);
	REQUIRE(is_array_v<B[]>);
	REQUIRE(is_array_v<B[4]>);
	REQUIRE(is_array_v<int[]>);
	REQUIRE(is_array_v<int[8]>);
	REQUIRE(is_array_v<bool[]>);
	REQUIRE(is_array_v<bool[16]>);
}

using substrate::is_enum_v;
TEST_CASE( "[C++ 17] is_enum_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	enum struct C : char {};

	REQUIRE_FALSE(is_enum_v<A>);
	REQUIRE_FALSE(is_enum_v<int>);
	REQUIRE_FALSE(is_enum_v<bool>);

	REQUIRE(is_enum_v<B>);
	REQUIRE(is_enum_v<C>);

}

using substrate::is_union_v;
TEST_CASE( "[C++ 17] is_union_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	typedef union { int a; float b; } C;
	struct D { C c; };

	REQUIRE_FALSE(is_union_v<A>);
	REQUIRE_FALSE(is_union_v<B>);
	REQUIRE_FALSE(is_union_v<D>);
	REQUIRE_FALSE(is_union_v<int>);
	REQUIRE_FALSE(is_union_v<void>);

	REQUIRE(is_union_v<C>);
}

using substrate::is_class_v;
TEST_CASE( "[C++ 17] is_class_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	typedef union { int a; float b; } C;
	struct D { C c; };
	class E : A {};
	enum class F {};

	REQUIRE_FALSE(is_class_v<B>);
	REQUIRE_FALSE(is_class_v<C>);
	REQUIRE_FALSE(is_class_v<F>);
	REQUIRE_FALSE(is_class_v<int>);
	REQUIRE_FALSE(is_class_v<void>);

	REQUIRE(is_class_v<A>);
	REQUIRE(is_class_v<D>);
	REQUIRE(is_class_v<E>);
}

template<typename>
struct SU_traits {};

template<class T, class U>
struct SU_traits<U T::*> {
    using member_type = U;
};

using substrate::is_function_v;
TEST_CASE( "[C++ 17] is_function_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	struct C { int foo() const&; };
	int bar();

	REQUIRE_FALSE(is_function_v<A>);
	REQUIRE_FALSE(is_function_v<B>);
	REQUIRE_FALSE(is_function_v<C>);
	REQUIRE_FALSE(is_function_v<int>);
	REQUIRE_FALSE(is_function_v<void>);

	REQUIRE(is_function_v<int(void)>);
	REQUIRE(is_function_v<void(int)>);
	REQUIRE(is_function_v<decltype(bar)>);
	REQUIRE(is_function_v<SU_traits<decltype(&C::foo)>::member_type>);
}

using substrate::is_pointer_v;
TEST_CASE( "[C++ 17] is_pointer_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_pointer_v<A>);
	REQUIRE_FALSE(is_pointer_v<B&>);
	REQUIRE_FALSE(is_pointer_v<int>);
	REQUIRE_FALSE(is_pointer_v<void>);
	REQUIRE_FALSE(is_pointer_v<char[]>);
	REQUIRE_FALSE(is_pointer_v<bool[1]>);
	REQUIRE_FALSE(is_pointer_v<std::nullptr_t>);

	REQUIRE(is_pointer_v<A*>);
	REQUIRE(is_pointer_v<int*>);
	REQUIRE(is_pointer_v<void**>);
}

using substrate::is_lvalue_reference_v;
TEST_CASE( "[C++ 17] is_lvalue_reference_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_lvalue_reference_v<A>);
	REQUIRE_FALSE(is_lvalue_reference_v<A&&>);
	REQUIRE_FALSE(is_lvalue_reference_v<int>);
	REQUIRE_FALSE(is_lvalue_reference_v<int&&>);

	REQUIRE(is_lvalue_reference_v<A&>);
	REQUIRE(is_lvalue_reference_v<int&>);
}

using substrate::is_rvalue_reference_v;
TEST_CASE( "[C++ 17] is_rvalue_reference_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_rvalue_reference_v<A>);
	REQUIRE_FALSE(is_rvalue_reference_v<A&>);
	REQUIRE_FALSE(is_rvalue_reference_v<int>);
	REQUIRE_FALSE(is_rvalue_reference_v<int&>);

	REQUIRE(is_rvalue_reference_v<A&&>);
	REQUIRE(is_rvalue_reference_v<int&&>);
}

using substrate::is_member_object_pointer_v;
TEST_CASE( "[C++ 17] is_member_object_pointer_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_member_object_pointer_v<int(A::*)()>);

	REQUIRE(is_member_object_pointer_v<int(A::*)>);
}

using substrate::is_member_function_pointer_v;
TEST_CASE( "[C++ 17] is_member_function_pointer_v helper", "[utility]")
{
	struct A { void foo() { } };

	REQUIRE(is_member_function_pointer_v<decltype(&A::foo)>);
}

using substrate::is_fundamental_v;
TEST_CASE( "[C++ 17] is_fundamental_v helper", "[utility]")
{
	class A {};
	int bar();

	REQUIRE_FALSE(is_fundamental_v<A>);
	REQUIRE_FALSE(is_fundamental_v<A*>);
	REQUIRE_FALSE(is_fundamental_v<A&>);
	REQUIRE_FALSE(is_fundamental_v<A&&>);
	REQUIRE_FALSE(is_fundamental_v<int*>);
	REQUIRE_FALSE(is_fundamental_v<int&>);
	REQUIRE_FALSE(is_fundamental_v<int&&>);
	REQUIRE_FALSE(is_fundamental_v<decltype(bar)>);

	REQUIRE(is_fundamental_v<int>);
	REQUIRE(is_fundamental_v<float>);
	REQUIRE(is_fundamental_v<bool>);
}

using substrate::is_arithmetic_v;
TEST_CASE( "[C++ 17] is_arithmetic_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_arithmetic_v<A>);
	REQUIRE_FALSE(is_arithmetic_v<B>);
	REQUIRE_FALSE(is_arithmetic_v<int*>);
	REQUIRE_FALSE(is_arithmetic_v<int&>);
	REQUIRE_FALSE(is_arithmetic_v<float*>);
	REQUIRE_FALSE(is_arithmetic_v<float&>);

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
TEST_CASE( "[C++ 17] is_object_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_object_v<A&>);
	REQUIRE_FALSE(is_object_v<A&&>);
	REQUIRE_FALSE(is_object_v<B&>);
	REQUIRE_FALSE(is_object_v<B&&>);
	REQUIRE_FALSE(is_object_v<int&>);
	REQUIRE_FALSE(is_object_v<int&&>);

	REQUIRE(is_object_v<A>);
	REQUIRE(is_object_v<B>);
	REQUIRE(is_object_v<int>);
	REQUIRE(is_object_v<float>);
}

using substrate::is_compound_v;
TEST_CASE( "[C++ 17] is_compound_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_compound_v<int>);

	REQUIRE(is_compound_v<A>);
}

using substrate::is_reference_v;
TEST_CASE( "[C++ 17] is_reference_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_reference_v<A>);
	REQUIRE_FALSE(is_reference_v<int>);

	REQUIRE(is_reference_v<A&>);
	REQUIRE(is_reference_v<A&&>);
	REQUIRE(is_reference_v<int&>);
	REQUIRE(is_reference_v<int&&>);
}

using substrate::is_member_pointer_v;
TEST_CASE( "[C++ 17] is_member_pointer_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_member_pointer_v<int>);

	REQUIRE(is_member_pointer_v<int(A::*)>);
}

using substrate::is_scalar_v;
TEST_CASE( "[C++ 17] is_scalar_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_scalar_v<A>);

	REQUIRE(is_scalar_v<B>);
	REQUIRE(is_scalar_v<int>);
	REQUIRE(is_scalar_v<float>);
	REQUIRE(is_scalar_v<bool>);
	REQUIRE(is_scalar_v<char>);
}

using substrate::is_base_of_v;
TEST_CASE( "[C++ 17] is_base_of_v helper", "[utility]")
{
	class A {};
	class B : A {};
	class C : B {};
	class D {};

	REQUIRE_FALSE(is_base_of_v<D, A>);
	REQUIRE_FALSE(is_base_of_v<B, A>);
	REQUIRE_FALSE(is_base_of_v<C, A>);
	REQUIRE_FALSE(is_base_of_v<C ,B>);

	REQUIRE(is_base_of_v<A, B>);
	REQUIRE(is_base_of_v<A, C>);
	REQUIRE(is_base_of_v<B, C>);
}

using substrate::is_convertible_v;
TEST_CASE( "[C++ 17] is_convertible_v helper", "[utility]")
{
	class A {};
	class B : public A {};
	class C { public: A a; operator A() { return a; } };

	REQUIRE_FALSE(is_convertible_v<A, B>);
	REQUIRE_FALSE(is_convertible_v<B, C>);

	REQUIRE(is_convertible_v<B, A>);
}

using substrate::has_virtual_destructor_v;
TEST_CASE( "[C++ 17] has_virtual_destructor_v helper", "[utility]")
{
	class A { ~A() { } };
	class B { virtual ~B() = 0; };

	REQUIRE_FALSE(has_virtual_destructor_v<A>);

	REQUIRE(has_virtual_destructor_v<B>);
}

using substrate::is_destructible_v;
TEST_CASE( "[C++ 17] is_destructible_v helper", "[utility]")
{
	struct A { std::string foo; ~A() noexcept {} };
	struct B { ~B() = default; };

	REQUIRE(is_destructible_v<A>);
	REQUIRE(is_destructible_v<B>);
}

using substrate::is_trivially_destructible_v;
TEST_CASE( "[C++ 17] is_trivially_destructible_v helper", "[utility]")
{
	struct A { std::string foo; ~A() noexcept {} };
	struct B { ~B() = default; };

	REQUIRE_FALSE(is_trivially_destructible_v<A>);

	REQUIRE(is_trivially_destructible_v<B>);
}

using substrate::is_nothrow_destructible_v;
TEST_CASE( "[C++ 17] is_nothrow_destructible_v helper", "[utility]")
{
	struct A { std::string foo; ~A() noexcept {} };
	struct B { ~B() = default; };

	REQUIRE(is_nothrow_destructible_v<A>);
	REQUIRE(is_nothrow_destructible_v<B>);
}

using substrate::is_move_assignable_v;
TEST_CASE( "[C++ 17] is_move_assignable_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { B& operator=(const B&) { return *this; }; };

	REQUIRE_FALSE(is_move_assignable_v<int[]>);
	REQUIRE_FALSE(is_move_assignable_v<int[4]>);

	REQUIRE(is_move_assignable_v<A>);
	REQUIRE(is_move_assignable_v<B>);
	REQUIRE(is_move_assignable_v<int>);
}

using substrate::is_trivially_move_assignable_v;
TEST_CASE( "[C++ 17] is_trivially_move_assignable_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { B& operator=(const B&) { return *this; }; };

	REQUIRE_FALSE(is_trivially_move_assignable_v<B>);
	REQUIRE_FALSE(is_trivially_move_assignable_v<int[]>);
	REQUIRE_FALSE(is_trivially_move_assignable_v<int[4]>);

	REQUIRE(is_trivially_move_assignable_v<A>);
	REQUIRE(is_trivially_move_assignable_v<int>);
}

using substrate::is_nothrow_move_assignable_v;
TEST_CASE( "[C++ 17] is_nothrow_move_assignable_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { B& operator=(const B&) { return *this; }; };

	REQUIRE_FALSE(is_nothrow_move_assignable_v<B>);
	REQUIRE_FALSE(is_nothrow_move_assignable_v<int[]>);
	REQUIRE_FALSE(is_nothrow_move_assignable_v<int[4]>);

	REQUIRE(is_nothrow_move_assignable_v<A>);
	REQUIRE(is_nothrow_move_assignable_v<int>);
}

using substrate::is_copy_assignable_v;
TEST_CASE( "[C++ 17] is_copy_assignable_v helper", "[utility]")
{
	struct A { int foo; };

	REQUIRE_FALSE(is_copy_assignable_v<int[]>);
	REQUIRE_FALSE(is_copy_assignable_v<int[4]>);

	REQUIRE(is_copy_assignable_v<A>);
	REQUIRE(is_copy_assignable_v<int>);
}

using substrate::is_trivially_copy_assignable_v;
TEST_CASE( "[C++ 17] is_trivially_copy_assignable_v helper", "[utility]")
{
	struct A { int foo; };

	REQUIRE_FALSE(is_trivially_copy_assignable_v<int[]>);
	REQUIRE_FALSE(is_trivially_copy_assignable_v<int[4]>);

	REQUIRE(is_trivially_copy_assignable_v<A>);
	REQUIRE(is_trivially_copy_assignable_v<int>);
}

using substrate::is_nothrow_copy_assignable_v;
TEST_CASE( "[C++ 17] is_nothrow_copy_assignable_v helper", "[utility]")
{
	struct A { int foo; };

	REQUIRE_FALSE(is_nothrow_copy_assignable_v<int[]>);
	REQUIRE_FALSE(is_nothrow_copy_assignable_v<int[4]>);

	REQUIRE(is_nothrow_copy_assignable_v<A>);
	REQUIRE(is_nothrow_copy_assignable_v<int>);
}

using substrate::is_assignable_v;
TEST_CASE( "[C++ 17] is_assignable_v helper", "[utility]")
{
	struct A { int foo{}; };
	struct B { int bar{}; B(int baz) { if (!baz) throw baz; } };
	struct C { B qux; };

	REQUIRE_FALSE(is_assignable_v<C, int>);
	REQUIRE_FALSE(is_assignable_v<A, int>);
	REQUIRE_FALSE(is_assignable_v<int, int>);
	REQUIRE_FALSE(is_assignable_v<int, int&>);
	REQUIRE_FALSE(is_assignable_v<int, float>);

	REQUIRE(is_assignable_v<A&, A&>);
	REQUIRE(is_assignable_v<B, int>);
	REQUIRE(is_assignable_v<int&, int>);
	REQUIRE(is_assignable_v<int&, int&>);
	REQUIRE(is_assignable_v<int&, float>);
}

using substrate::is_trivially_assignable_v;
TEST_CASE( "[C++ 17] is_trivially_assignable_v helper", "[utility]")
{
	struct A { int foo{}; };
	struct B { int bar{}; B(int baz) { if (!baz) throw baz; } };
	struct C { B qux; };

	REQUIRE_FALSE(is_trivially_assignable_v<B, int>);
	REQUIRE_FALSE(is_trivially_assignable_v<C, int>);
	REQUIRE_FALSE(is_trivially_assignable_v<A, int>);
	REQUIRE_FALSE(is_trivially_assignable_v<int, int>);
	REQUIRE_FALSE(is_trivially_assignable_v<int, int&>);
	REQUIRE_FALSE(is_trivially_assignable_v<int, float>);

	REQUIRE(is_trivially_assignable_v<A&, A&>);
	REQUIRE(is_trivially_assignable_v<int&, int>);
	REQUIRE(is_trivially_assignable_v<int&, int&>);
	REQUIRE(is_trivially_assignable_v<int&, float>);
}

using substrate::is_nothrow_assignable_v;
TEST_CASE( "[C++ 17] is_nothrow_assignable_v helper", "[utility]")
{
	struct A { int foo{}; };
	struct B { int bar{}; B(int baz) { if (!baz) throw baz; } };
	struct C { B qux; };

	REQUIRE_FALSE(is_nothrow_assignable_v<B, int>);
	REQUIRE_FALSE(is_nothrow_assignable_v<C, int>);
	REQUIRE_FALSE(is_nothrow_assignable_v<A, int>);
	REQUIRE_FALSE(is_nothrow_assignable_v<int, int>);
	REQUIRE_FALSE(is_nothrow_assignable_v<int, int&>);
	REQUIRE_FALSE(is_nothrow_assignable_v<int, float>);

	REQUIRE(is_nothrow_assignable_v<A&, A&>);
	REQUIRE(is_nothrow_assignable_v<int&, int>);
	REQUIRE(is_nothrow_assignable_v<int&, int&>);
	REQUIRE(is_nothrow_assignable_v<int&, float>);
}

using substrate::is_move_constructible_v;
TEST_CASE( "[C++ 17] is_move_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(B&&) = default; };
	struct C { C(C&&) { throw this; } };
	struct D { C qux; };
	struct E { E(E&&) = delete; };

	REQUIRE_FALSE(is_move_constructible_v<E>);

	REQUIRE(is_move_constructible_v<A>);
	REQUIRE(is_move_constructible_v<C>);
	REQUIRE(is_move_constructible_v<D>);
	REQUIRE(is_move_constructible_v<B>);
}

using substrate::is_trivially_move_constructible_v;
TEST_CASE( "[C++ 17] is_trivially_move_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(B&&) = default; };
	struct C { C(C&&) { throw this; } };
	struct D { C qux; };
	struct E { E(E&&) = delete; };

	REQUIRE_FALSE(is_trivially_move_constructible_v<A>);
	REQUIRE_FALSE(is_trivially_move_constructible_v<C>);
	REQUIRE_FALSE(is_trivially_move_constructible_v<D>);
	REQUIRE_FALSE(is_trivially_move_constructible_v<E>);

	REQUIRE(is_trivially_move_constructible_v<B>);
}

using substrate::is_nothrow_move_constructible_v;
TEST_CASE( "[C++ 17] is_nothrow_move_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(B&&) = default; };
	struct C { C(C&&) { throw this; } };
	struct D { C qux; };
	struct E { E(E&&) = delete; };

	REQUIRE_FALSE(is_nothrow_move_constructible_v<C>);
	REQUIRE_FALSE(is_nothrow_move_constructible_v<D>);
	REQUIRE_FALSE(is_nothrow_move_constructible_v<E>);

	REQUIRE(is_nothrow_move_constructible_v<A>);
	REQUIRE(is_nothrow_move_constructible_v<B>);
}

using substrate::is_copy_constructible_v;
TEST_CASE( "[C++ 17] is_copy_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(const B&) = default; };
	struct C { C(const C&) { throw this; } };
	struct D { C qux; };
	struct E { E(const E&) = delete; };

	REQUIRE_FALSE(is_copy_constructible_v<E>);

	REQUIRE(is_copy_constructible_v<A>);
	REQUIRE(is_copy_constructible_v<C>);
	REQUIRE(is_copy_constructible_v<D>);
	REQUIRE(is_copy_constructible_v<B>);
}

using substrate::is_trivially_copy_constructible_v;
TEST_CASE( "[C++ 17] is_trivially_copy_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(const B&) = default; };
	struct C { C(const C&) { throw this; } };
	struct D { C qux; };
	struct E { E(const E&) = delete; };

	REQUIRE_FALSE(is_trivially_copy_constructible_v<A>);
	REQUIRE_FALSE(is_trivially_copy_constructible_v<C>);
	REQUIRE_FALSE(is_trivially_copy_constructible_v<D>);
	REQUIRE_FALSE(is_trivially_copy_constructible_v<E>);

	REQUIRE(is_trivially_copy_constructible_v<B>);
}

using substrate::is_nothrow_copy_constructible_v;
TEST_CASE( "[C++ 17] is_nothrow_copy_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(const B&) = default; };
	struct C { C(const C&) { throw this; } };
	struct D { C qux; };
	struct E { E(const E&) = delete; };

	REQUIRE_FALSE(is_nothrow_copy_constructible_v<E>);
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<A>);
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<C>);
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<D>);

	REQUIRE(is_nothrow_copy_constructible_v<B>);
}

using substrate::is_default_constructible_v;
TEST_CASE( "[C++ 17] is_default_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B() = default; };
	struct C { C(int baz) { if (!baz) throw baz; } };
	struct D { C qux; };
	struct E { E() = delete; };


	REQUIRE_FALSE(is_default_constructible_v<C>);
	REQUIRE_FALSE(is_default_constructible_v<D>);
	REQUIRE_FALSE(is_default_constructible_v<E>);

	REQUIRE(is_default_constructible_v<A>);
	REQUIRE(is_default_constructible_v<B>);
}

using substrate::is_trivially_default_constructible_v;
TEST_CASE( "[C++ 17] is_trivially_default_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B() = default; };
	struct C { C(int baz) { if (!baz) throw baz; } };
	struct D { C qux; };
	struct E { E() = delete; };

	REQUIRE_FALSE(is_trivially_default_constructible_v<A>);
	REQUIRE_FALSE(is_trivially_default_constructible_v<C>);
	REQUIRE_FALSE(is_trivially_default_constructible_v<D>);
	REQUIRE_FALSE(is_default_constructible_v<E>);

	REQUIRE(is_trivially_default_constructible_v<B>);
}

using substrate::is_nothrow_default_constructible_v;
TEST_CASE( "[C++ 17] is_nothrow_default_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B() = default; };
	struct C { C(int baz) { if (!baz) throw baz; } };
	struct D { C qux; };
	struct E { E() = delete; };

	REQUIRE_FALSE(is_nothrow_default_constructible_v<C>);
	REQUIRE_FALSE(is_nothrow_default_constructible_v<D>);
	REQUIRE_FALSE(is_nothrow_default_constructible_v<E>);

	REQUIRE(is_nothrow_default_constructible_v<A>);
	REQUIRE(is_nothrow_default_constructible_v<B>);
}

using substrate::is_constructible_v;
TEST_CASE( "[C++ 17] is_constructible_v helper", "[utility]")
{
	class A {
		int foo;
		double bar;
	public:
		A(int baz) : foo(baz), bar() { }
		A(int qux, double fwibble) noexcept : foo(qux), bar(fwibble) {}
	};

	class B {};

	REQUIRE_FALSE(is_constructible_v<A, B>);

	REQUIRE(is_constructible_v<A, int>);
	REQUIRE(is_constructible_v<A, int, double>);
}

using substrate::is_trivially_constructible_v;
TEST_CASE( "[C++ 17] is_trivially_constructible_v helper", "[utility]")
{
	class A {
		int foo;
		double bar;
	public:
		A(int baz) : foo(baz), bar() { }
		A(int qux, double fwibble) noexcept : foo(qux), bar(fwibble) {}
	};

	REQUIRE_FALSE(is_trivially_constructible_v<A, int>);

	REQUIRE(is_trivially_constructible_v<A, const A&>);
}

using substrate::is_nothrow_constructible_v;
TEST_CASE( "[C++ 17] is_nothrow_constructible_v helper", "[utility]")
{
	class A {
		int foo;
		double bar;
	public:
		A(int baz) : foo(baz), bar() { }
		A(int qux, double fwibble) noexcept : foo(qux), bar(fwibble) {}
	};

	REQUIRE_FALSE(is_nothrow_constructible_v<A, int>);

	REQUIRE(is_nothrow_constructible_v<A, int, double>);
}

using substrate::is_unsigned_v;
TEST_CASE( "[C++ 17] is_unsigned_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	enum struct C : int {};
	enum D : unsigned int {};
	enum struct E : unsigned int {};

	REQUIRE_FALSE(is_unsigned_v<A>);
	REQUIRE_FALSE(is_unsigned_v<B>);
	REQUIRE_FALSE(is_unsigned_v<C>);
	REQUIRE_FALSE(is_unsigned_v<D>);
	REQUIRE_FALSE(is_unsigned_v<E>);
	REQUIRE_FALSE(is_unsigned_v<int>);
	REQUIRE_FALSE(is_unsigned_v<signed int>);

	REQUIRE(is_unsigned_v<unsigned int>);
}

using substrate::is_signed_v;
TEST_CASE( "[C++ 17] is_signed_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	enum struct C : int {};
	enum D : unsigned int {};
	enum struct E : unsigned int {};

	REQUIRE_FALSE(is_signed_v<A>);
	REQUIRE_FALSE(is_signed_v<B>);
	REQUIRE_FALSE(is_signed_v<C>);
	REQUIRE_FALSE(is_signed_v<D>);
	REQUIRE_FALSE(is_signed_v<E>);
	REQUIRE_FALSE(is_signed_v<unsigned int>);

	REQUIRE(is_signed_v<int>);
	REQUIRE(is_signed_v<signed int>);
}

using substrate::is_abstract_v;
TEST_CASE( "[C++ 17] is_abstract_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { virtual void bar(); virtual ~B() = default; };
	struct C { virtual void baz() = 0; virtual ~C() = default; };
	struct D : C {};

	REQUIRE_FALSE(is_abstract_v<A>);
	REQUIRE_FALSE(is_abstract_v<B>);

	REQUIRE(is_abstract_v<C>);
	REQUIRE(is_abstract_v<D>);
}

using substrate::is_polymorphic_v;
TEST_CASE( "[C++ 17] is_polymorphic_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { virtual void bar(); virtual ~B() = default; };
	struct C : B {};

	REQUIRE_FALSE(is_polymorphic_v<A>);

	REQUIRE(is_polymorphic_v<B>);
	REQUIRE(is_polymorphic_v<C>);
}

struct SU_is_empty_v_C { static int bar; };
using substrate::is_empty_v;
TEST_CASE( "[C++ 17] is_empty_v helper", "[utility]")
{
	struct A {};
	struct B { int foo; };
	struct D { virtual ~D(); };

	enum E : int {};
	union F {};

	REQUIRE_FALSE(is_empty_v<B>);
	REQUIRE_FALSE(is_empty_v<D>);
	REQUIRE_FALSE(is_empty_v<E>);
	REQUIRE_FALSE(is_empty_v<F>);

	REQUIRE(is_empty_v<A>);
	REQUIRE(is_empty_v<SU_is_empty_v_C>);
}

using substrate::is_pod_v;
TEST_CASE( "[C++ 17] is_pod_v helper", "[utility]")
{
	struct A { int foo; };
	struct B {
		int bar;
	private:
		int baz;
	};
	struct C { virtual void qux(); virtual ~C() = default;  };

	REQUIRE_FALSE(is_pod_v<B>);
	REQUIRE_FALSE(is_pod_v<C>);

	REQUIRE(is_pod_v<A>);
}

using substrate::is_standard_layout_v;
TEST_CASE( "[C++ 17] is_standard_layout_v helper", "[utility]")
{
	struct A { int foo; };
	struct B {
		int bar;
	private:
		int baz;
	};
	struct C { virtual void qux(); virtual ~C() = default;  };

	REQUIRE_FALSE(is_standard_layout_v<B>);
	REQUIRE_FALSE(is_standard_layout_v<C>);

	REQUIRE(is_standard_layout_v<A>);
}

using substrate::is_trivially_copyable_v;
TEST_CASE( "[C++ 17] is_trivially_copyable_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { B(B const&) {} };
	struct C { virtual void baz(); virtual ~C() = default; };
	struct D { int foo; D(D const&) = default; D(int qux) : foo(qux+1) {} };

	REQUIRE_FALSE(is_trivially_copyable_v<B>);
	REQUIRE_FALSE(is_trivially_copyable_v<C>);

	REQUIRE(is_trivially_copyable_v<A>);
	REQUIRE(is_trivially_copyable_v<D>);
}

using substrate::is_trivial_v;
TEST_CASE( "[C++ 17] is_trivial_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { B() {} };

	REQUIRE_FALSE(is_trivial_v<B>);

	REQUIRE(is_trivial_v<A>);
}

using substrate::is_volatile_v;
TEST_CASE( "[C++ 17] is_volatile_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_volatile_v<A>);
	REQUIRE_FALSE(is_volatile_v<int>);

	REQUIRE(is_volatile_v<volatile int>);
}

using substrate::is_const_v;
TEST_CASE( "[C++ 17] is_const_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_const_v<A>);
	REQUIRE_FALSE(is_const_v<int>);
	REQUIRE_FALSE(is_const_v<const int*>);
	REQUIRE_FALSE(is_const_v<const int&>);

	REQUIRE(is_const_v<const int>);
	REQUIRE(is_const_v<int* const>);
	REQUIRE(is_const_v<substrate::remove_reference_t<const int&>>);
}

using substrate::is_literal_type_v;
TEST_CASE( "[C++ 17] is_literal_type_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { virtual ~B(); };

	REQUIRE_FALSE(is_literal_type_v<B>);

	REQUIRE(is_literal_type_v<A>);
}

#elif defined(SUBSTRATE_CXX11_COMPAT)

using substrate::is_same_v;
TEST_CASE( "[C++ 17] is_same_v helper", "[utility]")
{
	class A {};

	using uint = unsigned int;
	using sint = signed int;

	REQUIRE_FALSE(is_same_v<int, bool>());
	REQUIRE_FALSE(is_same_v<float, int>());
	REQUIRE_FALSE(is_same_v<A, int>());
	REQUIRE_FALSE(is_same_v<uint, sint>());

	REQUIRE(is_same_v<A, A>());
	REQUIRE(is_same_v<uint, unsigned int>());
	REQUIRE(is_same_v<sint, signed int>());
}

using substrate::is_null_pointer_v;
TEST_CASE( "[C++ 17] is_null_pointer_v helper", "[utility]")
{
	REQUIRE(is_null_pointer_v<decltype(nullptr)>());
	REQUIRE_FALSE(is_null_pointer_v<int*>());
}

using substrate::is_void_v;
TEST_CASE( "[C++ 17] is_void_v helper", "[utility]")
{
	REQUIRE(is_void_v<void>());
	REQUIRE_FALSE(is_void_v<int>());
}

using substrate::is_integral_v;
TEST_CASE( "[C++ 17] is_integral_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_integral_v<A>());
	REQUIRE_FALSE(is_integral_v<B>());
	REQUIRE_FALSE(is_integral_v<float>());
	REQUIRE_FALSE(is_integral_v<double>());

	REQUIRE(is_integral_v<int>());
	REQUIRE(is_integral_v<bool>());
	REQUIRE(is_integral_v<unsigned int>());
	REQUIRE(is_integral_v<unsigned char>());

}

using substrate::is_floating_point_v;
TEST_CASE( "[C++ 17] is_floating_point_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_floating_point_v<A>());
	REQUIRE_FALSE(is_floating_point_v<B>());
	REQUIRE_FALSE(is_floating_point_v<int>());
	REQUIRE_FALSE(is_floating_point_v<bool>());
	REQUIRE_FALSE(is_floating_point_v<unsigned int>());
	REQUIRE_FALSE(is_floating_point_v<unsigned char>());

	REQUIRE(is_floating_point_v<float>());
	REQUIRE(is_floating_point_v<double>());
}

using substrate::is_array_v;
TEST_CASE( "[C++ 17] is_array_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_array_v<A>());
	REQUIRE_FALSE(is_array_v<B>());
	REQUIRE_FALSE(is_array_v<int>());
	REQUIRE_FALSE(is_array_v<bool>());

	REQUIRE(is_array_v<A[]>());
	REQUIRE(is_array_v<A[2]>());
	REQUIRE(is_array_v<B[]>());
	REQUIRE(is_array_v<B[4]>());
	REQUIRE(is_array_v<int[]>());
	REQUIRE(is_array_v<int[8]>());
	REQUIRE(is_array_v<bool[]>());
	REQUIRE(is_array_v<bool[16]>());
}

using substrate::is_enum_v;
TEST_CASE( "[C++ 17] is_enum_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	enum struct C : char {};

	REQUIRE_FALSE(is_enum_v<A>());
	REQUIRE_FALSE(is_enum_v<int>());
	REQUIRE_FALSE(is_enum_v<bool>());

	REQUIRE(is_enum_v<B>());
	REQUIRE(is_enum_v<C>());

}

using substrate::is_union_v;
TEST_CASE( "[C++ 17] is_union_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	typedef union { int a; float b; } C;
	struct D { C c; };

	REQUIRE_FALSE(is_union_v<A>());
	REQUIRE_FALSE(is_union_v<B>());
	REQUIRE_FALSE(is_union_v<D>());
	REQUIRE_FALSE(is_union_v<int>());
	REQUIRE_FALSE(is_union_v<void>());

	REQUIRE(is_union_v<C>());
}

using substrate::is_class_v;
TEST_CASE( "[C++ 17] is_class_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	typedef union { int a; float b; } C;
	struct D { C c; };
	class E : A {};
	enum class F {};

	REQUIRE_FALSE(is_class_v<B>());
	REQUIRE_FALSE(is_class_v<C>());
	REQUIRE_FALSE(is_class_v<F>());
	REQUIRE_FALSE(is_class_v<int>());
	REQUIRE_FALSE(is_class_v<void>());

	REQUIRE(is_class_v<A>());
	REQUIRE(is_class_v<D>());
	REQUIRE(is_class_v<E>());
}

template<typename>
struct SU_traits {};

template<class T, class U>
struct SU_traits<U T::*> {
    using member_type = U;
};

using substrate::is_function_v;
TEST_CASE( "[C++ 17] is_function_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	struct C { int foo() const&; };
	int bar();

	REQUIRE_FALSE(is_function_v<A>());
	REQUIRE_FALSE(is_function_v<B>());
	REQUIRE_FALSE(is_function_v<C>());
	REQUIRE_FALSE(is_function_v<int>());
	REQUIRE_FALSE(is_function_v<void>());

	REQUIRE(is_function_v<int(void)>());
	REQUIRE(is_function_v<void(int)>());
	REQUIRE(is_function_v<decltype(bar)>());
	REQUIRE(is_function_v<SU_traits<decltype(&C::foo)>::member_type>());
}

using substrate::is_pointer_v;
TEST_CASE( "[C++ 17] is_pointer_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_pointer_v<A>());
	REQUIRE_FALSE(is_pointer_v<B&>());
	REQUIRE_FALSE(is_pointer_v<int>());
	REQUIRE_FALSE(is_pointer_v<void>());
	REQUIRE_FALSE(is_pointer_v<char[]>());
	REQUIRE_FALSE(is_pointer_v<bool[1]>());
	REQUIRE_FALSE(is_pointer_v<std::nullptr_t>());

	REQUIRE(is_pointer_v<A*>());
	REQUIRE(is_pointer_v<int*>());
	REQUIRE(is_pointer_v<void**>());
}

using substrate::is_lvalue_reference_v;
TEST_CASE( "[C++ 17] is_lvalue_reference_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_lvalue_reference_v<A>());
	REQUIRE_FALSE(is_lvalue_reference_v<A&&>());
	REQUIRE_FALSE(is_lvalue_reference_v<int>());
	REQUIRE_FALSE(is_lvalue_reference_v<int&&>());

	REQUIRE(is_lvalue_reference_v<A&>());
	REQUIRE(is_lvalue_reference_v<int&>());
}

using substrate::is_rvalue_reference_v;
TEST_CASE( "[C++ 17] is_rvalue_reference_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_rvalue_reference_v<A>());
	REQUIRE_FALSE(is_rvalue_reference_v<A&>());
	REQUIRE_FALSE(is_rvalue_reference_v<int>());
	REQUIRE_FALSE(is_rvalue_reference_v<int&>());

	REQUIRE(is_rvalue_reference_v<A&&>());
	REQUIRE(is_rvalue_reference_v<int&&>());
}

using substrate::is_member_object_pointer_v;
TEST_CASE( "[C++ 17] is_member_object_pointer_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_member_object_pointer_v<int(A::*)()>());

	REQUIRE(is_member_object_pointer_v<int(A::*)>());
}

using substrate::is_member_function_pointer_v;
TEST_CASE( "[C++ 17] is_member_function_pointer_v helper", "[utility]")
{
	struct A { void foo() { } };

	REQUIRE(is_member_function_pointer_v<decltype(&A::foo)>());
}

using substrate::is_fundamental_v;
TEST_CASE( "[C++ 17] is_fundamental_v helper", "[utility]")
{
	class A {};
	int bar();

	REQUIRE_FALSE(is_fundamental_v<A>());
	REQUIRE_FALSE(is_fundamental_v<A*>());
	REQUIRE_FALSE(is_fundamental_v<A&>());
	REQUIRE_FALSE(is_fundamental_v<A&&>());
	REQUIRE_FALSE(is_fundamental_v<int*>());
	REQUIRE_FALSE(is_fundamental_v<int&>());
	REQUIRE_FALSE(is_fundamental_v<int&&>());
	REQUIRE_FALSE(is_fundamental_v<decltype(bar)>());

	REQUIRE(is_fundamental_v<int>());
	REQUIRE(is_fundamental_v<float>());
	REQUIRE(is_fundamental_v<bool>());
}

using substrate::is_arithmetic_v;
TEST_CASE( "[C++ 17] is_arithmetic_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_arithmetic_v<A>());
	REQUIRE_FALSE(is_arithmetic_v<B>());
	REQUIRE_FALSE(is_arithmetic_v<int*>());
	REQUIRE_FALSE(is_arithmetic_v<int&>());
	REQUIRE_FALSE(is_arithmetic_v<float*>());
	REQUIRE_FALSE(is_arithmetic_v<float&>());

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
TEST_CASE( "[C++ 17] is_object_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_object_v<A&>());
	REQUIRE_FALSE(is_object_v<A&&>());
	REQUIRE_FALSE(is_object_v<B&>());
	REQUIRE_FALSE(is_object_v<B&&>());
	REQUIRE_FALSE(is_object_v<int&>());
	REQUIRE_FALSE(is_object_v<int&&>());

	REQUIRE(is_object_v<A>());
	REQUIRE(is_object_v<B>());
	REQUIRE(is_object_v<int>());
	REQUIRE(is_object_v<float>());
}

using substrate::is_compound_v;
TEST_CASE( "[C++ 17] is_compound_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_compound_v<int>());

	REQUIRE(is_compound_v<A>());
}

using substrate::is_reference_v;
TEST_CASE( "[C++ 17] is_reference_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_reference_v<A>());
	REQUIRE_FALSE(is_reference_v<int>());

	REQUIRE(is_reference_v<A&>());
	REQUIRE(is_reference_v<A&&>());
	REQUIRE(is_reference_v<int&>());
	REQUIRE(is_reference_v<int&&>());
}

using substrate::is_member_pointer_v;
TEST_CASE( "[C++ 17] is_member_pointer_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_member_pointer_v<int>());

	REQUIRE(is_member_pointer_v<int(A::*)>());
}

using substrate::is_scalar_v;
TEST_CASE( "[C++ 17] is_scalar_v helper", "[utility]")
{
	class A {};
	enum B : int {};

	REQUIRE_FALSE(is_scalar_v<A>());

	REQUIRE(is_scalar_v<B>());
	REQUIRE(is_scalar_v<int>());
	REQUIRE(is_scalar_v<float>());
	REQUIRE(is_scalar_v<bool>());
	REQUIRE(is_scalar_v<char>());
}

using substrate::is_base_of_v;
TEST_CASE( "[C++ 17] is_base_of_v helper", "[utility]")
{
	class A {};
	class B : A {};
	class C : B {};
	class D {};

	REQUIRE_FALSE(is_base_of_v<D, A>());
	REQUIRE_FALSE(is_base_of_v<B, A>());
	REQUIRE_FALSE(is_base_of_v<C, A>());
	REQUIRE_FALSE(is_base_of_v<C ,B>());

	REQUIRE(is_base_of_v<A, B>());
	REQUIRE(is_base_of_v<A, C>());
	REQUIRE(is_base_of_v<B, C>());
}

using substrate::is_convertible_v;
TEST_CASE( "[C++ 17] is_convertible_v helper", "[utility]")
{
	class A {};
	class B : public A {};
	class C { public: A a; operator A() { return a; } };

	REQUIRE_FALSE(is_convertible_v<A, B>());
	REQUIRE_FALSE(is_convertible_v<B, C>());

	REQUIRE(is_convertible_v<B, A>());
}

using substrate::has_virtual_destructor_v;
TEST_CASE( "[C++ 17] has_virtual_destructor_v helper", "[utility]")
{
	class A { ~A() { } };
	class B { virtual ~B() = 0; };

	REQUIRE_FALSE(has_virtual_destructor_v<A>());

	REQUIRE(has_virtual_destructor_v<B>());
}

using substrate::is_destructible_v;
TEST_CASE( "[C++ 17] is_destructible_v helper", "[utility]")
{
	struct A { std::string foo; ~A() noexcept {} };
	struct B { ~B() = default; };

	REQUIRE(is_destructible_v<A>());
	REQUIRE(is_destructible_v<B>());
}

using substrate::is_trivially_destructible_v;
TEST_CASE( "[C++ 17] is_trivially_destructible_v helper", "[utility]")
{
	struct A { std::string foo; ~A() noexcept {} };
	struct B { ~B() = default; };

	REQUIRE_FALSE(is_trivially_destructible_v<A>());

	REQUIRE(is_trivially_destructible_v<B>());
}

using substrate::is_nothrow_destructible_v;
TEST_CASE( "[C++ 17] is_nothrow_destructible_v helper", "[utility]")
{
	struct A { std::string foo; ~A() noexcept {} };
	struct B { ~B() = default; };

	REQUIRE(is_nothrow_destructible_v<A>());
	REQUIRE(is_nothrow_destructible_v<B>());
}

using substrate::is_move_assignable_v;
TEST_CASE( "[C++ 17] is_move_assignable_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { B& operator=(const B&) { return *this; }; };

	REQUIRE_FALSE(is_move_assignable_v<int[]>());
	REQUIRE_FALSE(is_move_assignable_v<int[4]>());

	REQUIRE(is_move_assignable_v<A>());
	REQUIRE(is_move_assignable_v<B>());
	REQUIRE(is_move_assignable_v<int>());
}

using substrate::is_trivially_move_assignable_v;
TEST_CASE( "[C++ 17] is_trivially_move_assignable_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { B& operator=(const B&) { return *this; }; };

	REQUIRE_FALSE(is_trivially_move_assignable_v<B>());
	REQUIRE_FALSE(is_trivially_move_assignable_v<int[]>());
	REQUIRE_FALSE(is_trivially_move_assignable_v<int[4]>());

	REQUIRE(is_trivially_move_assignable_v<A>());
	REQUIRE(is_trivially_move_assignable_v<int>());
}

using substrate::is_nothrow_move_assignable_v;
TEST_CASE( "[C++ 17] is_nothrow_move_assignable_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { B& operator=(const B&) { return *this; }; };

	REQUIRE_FALSE(is_nothrow_move_assignable_v<B>());
	REQUIRE_FALSE(is_nothrow_move_assignable_v<int[]>());
	REQUIRE_FALSE(is_nothrow_move_assignable_v<int[4]>());

	REQUIRE(is_nothrow_move_assignable_v<A>());
	REQUIRE(is_nothrow_move_assignable_v<int>());
}

using substrate::is_copy_assignable_v;
TEST_CASE( "[C++ 17] is_copy_assignable_v helper", "[utility]")
{
	struct A { int foo; };

	REQUIRE_FALSE(is_copy_assignable_v<int[]>());
	REQUIRE_FALSE(is_copy_assignable_v<int[4]>());

	REQUIRE(is_copy_assignable_v<A>());
	REQUIRE(is_copy_assignable_v<int>());
}

using substrate::is_trivially_copy_assignable_v;
TEST_CASE( "[C++ 17] is_trivially_copy_assignable_v helper", "[utility]")
{
	struct A { int foo; };

	REQUIRE_FALSE(is_trivially_copy_assignable_v<int[]>());
	REQUIRE_FALSE(is_trivially_copy_assignable_v<int[4]>());

	REQUIRE(is_trivially_copy_assignable_v<A>());
	REQUIRE(is_trivially_copy_assignable_v<int>());
}

using substrate::is_nothrow_copy_assignable_v;
TEST_CASE( "[C++ 17] is_nothrow_copy_assignable_v helper", "[utility]")
{
	struct A { int foo; };

	REQUIRE_FALSE(is_nothrow_copy_assignable_v<int[]>());
	REQUIRE_FALSE(is_nothrow_copy_assignable_v<int[4]>());

	REQUIRE(is_nothrow_copy_assignable_v<A>());
	REQUIRE(is_nothrow_copy_assignable_v<int>());
}

using substrate::is_assignable_v;
TEST_CASE( "[C++ 17] is_assignable_v helper", "[utility]")
{
	struct A { int foo{}; };
	struct B { int bar{}; B(int baz) { if (!baz) throw baz; } };
	struct C { B qux; };

	REQUIRE_FALSE(is_assignable_v<C, int>());
	REQUIRE_FALSE(is_assignable_v<A, int>());
	REQUIRE_FALSE(is_assignable_v<int, int>());
	REQUIRE_FALSE(is_assignable_v<int, int&>());
	REQUIRE_FALSE(is_assignable_v<int, float>());

	REQUIRE(is_assignable_v<A&, A&>());
	REQUIRE(is_assignable_v<B, int>());
	REQUIRE(is_assignable_v<int&, int>());
	REQUIRE(is_assignable_v<int&, int&>());
	REQUIRE(is_assignable_v<int&, float>());
}

using substrate::is_trivially_assignable_v;
TEST_CASE( "[C++ 17] is_trivially_assignable_v helper", "[utility]")
{
	struct A { int foo{}; };
	struct B { int bar{}; B(int baz) { if (!baz) throw baz; } };
	struct C { B qux; };

	REQUIRE_FALSE(is_trivially_assignable_v<B, int>());
	REQUIRE_FALSE(is_trivially_assignable_v<C, int>());
	REQUIRE_FALSE(is_trivially_assignable_v<A, int>());
	REQUIRE_FALSE(is_trivially_assignable_v<int, int>());
	REQUIRE_FALSE(is_trivially_assignable_v<int, int&>());
	REQUIRE_FALSE(is_trivially_assignable_v<int, float>());

	REQUIRE(is_trivially_assignable_v<A&, A&>());
	REQUIRE(is_trivially_assignable_v<int&, int>());
	REQUIRE(is_trivially_assignable_v<int&, int&>());
	REQUIRE(is_trivially_assignable_v<int&, float>());
}

using substrate::is_nothrow_assignable_v;
TEST_CASE( "[C++ 17] is_nothrow_assignable_v helper", "[utility]")
{
	struct A { int foo{}; };
	struct B { int bar{}; B(int baz) { if (!baz) throw baz; } };
	struct C { B qux; };

	REQUIRE_FALSE(is_nothrow_assignable_v<B, int>());
	REQUIRE_FALSE(is_nothrow_assignable_v<C, int>());
	REQUIRE_FALSE(is_nothrow_assignable_v<A, int>());
	REQUIRE_FALSE(is_nothrow_assignable_v<int, int>());
	REQUIRE_FALSE(is_nothrow_assignable_v<int, int&>());
	REQUIRE_FALSE(is_nothrow_assignable_v<int, float>());

	REQUIRE(is_nothrow_assignable_v<A&, A&>());
	REQUIRE(is_nothrow_assignable_v<int&, int>());
	REQUIRE(is_nothrow_assignable_v<int&, int&>());
	REQUIRE(is_nothrow_assignable_v<int&, float>());
}

using substrate::is_move_constructible_v;
TEST_CASE( "[C++ 17] is_move_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(B&&) = default; };
	struct C { C(C&&) { throw this; } };
	struct D { C qux; };
	struct E { E(E&&) = delete; };

	REQUIRE_FALSE(is_move_constructible_v<E>());

	REQUIRE(is_move_constructible_v<A>());
	REQUIRE(is_move_constructible_v<C>());
	REQUIRE(is_move_constructible_v<D>());
	REQUIRE(is_move_constructible_v<B>());
}

using substrate::is_trivially_move_constructible_v;
TEST_CASE( "[C++ 17] is_trivially_move_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(B&&) = default; };
	struct C { C(C&&) { throw this; } };
	struct D { C qux; };
	struct E { E(E&&) = delete; };

	REQUIRE_FALSE(is_trivially_move_constructible_v<A>());
	REQUIRE_FALSE(is_trivially_move_constructible_v<C>());
	REQUIRE_FALSE(is_trivially_move_constructible_v<D>());
	REQUIRE_FALSE(is_trivially_move_constructible_v<E>());

	REQUIRE(is_trivially_move_constructible_v<B>());
}

using substrate::is_nothrow_move_constructible_v;
TEST_CASE( "[C++ 17] is_nothrow_move_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(B&&) = default; };
	struct C { C(C&&) { throw this; } };
	struct D { C qux; };
	struct E { E(E&&) = delete; };

	REQUIRE_FALSE(is_nothrow_move_constructible_v<C>());
	REQUIRE_FALSE(is_nothrow_move_constructible_v<D>());
	REQUIRE_FALSE(is_nothrow_move_constructible_v<E>());

	REQUIRE(is_nothrow_move_constructible_v<A>());
	REQUIRE(is_nothrow_move_constructible_v<B>());
}

using substrate::is_copy_constructible_v;
TEST_CASE( "[C++ 17] is_copy_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(const B&) = default; };
	struct C { C(const C&) { throw this; } };
	struct D { C qux; };
	struct E { E(const E&) = delete; };

	REQUIRE_FALSE(is_copy_constructible_v<E>());

	REQUIRE(is_copy_constructible_v<A>());
	REQUIRE(is_copy_constructible_v<C>());
	REQUIRE(is_copy_constructible_v<D>());
	REQUIRE(is_copy_constructible_v<B>());
}

using substrate::is_trivially_copy_constructible_v;
TEST_CASE( "[C++ 17] is_trivially_copy_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(const B&) = default; };
	struct C { C(const C&) { throw this; } };
	struct D { C qux; };
	struct E { E(const E&) = delete; };

	REQUIRE_FALSE(is_trivially_copy_constructible_v<A>());
	REQUIRE_FALSE(is_trivially_copy_constructible_v<C>());
	REQUIRE_FALSE(is_trivially_copy_constructible_v<D>());
	REQUIRE_FALSE(is_trivially_copy_constructible_v<E>());

	REQUIRE(is_trivially_copy_constructible_v<B>());
}

using substrate::is_nothrow_copy_constructible_v;
TEST_CASE( "[C++ 17] is_nothrow_copy_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B(const B&) = default; };
	struct C { C(const C&) { throw this; } };
	struct D { C qux; };
	struct E { E(const E&) = delete; };

	REQUIRE_FALSE(is_nothrow_copy_constructible_v<E>());
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<A>());
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<C>());
	REQUIRE_FALSE(is_nothrow_copy_constructible_v<D>());

	REQUIRE(is_nothrow_copy_constructible_v<B>());
}

using substrate::is_default_constructible_v;
TEST_CASE( "[C++ 17] is_default_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B() = default; };
	struct C { C(int baz) { if (!baz) throw baz; } };
	struct D { C qux; };
	struct E { E() = delete; };


	REQUIRE_FALSE(is_default_constructible_v<C>());
	REQUIRE_FALSE(is_default_constructible_v<D>());
	REQUIRE_FALSE(is_default_constructible_v<E>());

	REQUIRE(is_default_constructible_v<A>());
	REQUIRE(is_default_constructible_v<B>());
}

using substrate::is_trivially_default_constructible_v;
TEST_CASE( "[C++ 17] is_trivially_default_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B() = default; };
	struct C { C(int baz) { if (!baz) throw baz; } };
	struct D { C qux; };
	struct E { E() = delete; };

	REQUIRE_FALSE(is_trivially_default_constructible_v<A>());
	REQUIRE_FALSE(is_trivially_default_constructible_v<C>());
	REQUIRE_FALSE(is_trivially_default_constructible_v<D>());
	REQUIRE_FALSE(is_trivially_default_constructible_v<E>());

	REQUIRE(is_trivially_default_constructible_v<B>());
}

using substrate::is_nothrow_default_constructible_v;
TEST_CASE( "[C++ 17] is_nothrow_default_constructible_v helper", "[utility]")
{
	struct A { std::string foo; };
	struct B { int bar; B() = default; };
	struct C { C(int baz) { if (!baz) throw baz; } };
	struct D { C qux; };
	struct E { E() = delete; };

	REQUIRE_FALSE(is_nothrow_default_constructible_v<C>());
	REQUIRE_FALSE(is_nothrow_default_constructible_v<D>());
	REQUIRE_FALSE(is_nothrow_default_constructible_v<E>());

	REQUIRE(is_nothrow_default_constructible_v<A>());
	REQUIRE(is_nothrow_default_constructible_v<B>());
}

using substrate::is_constructible_v;
TEST_CASE( "[C++ 17] is_constructible_v helper", "[utility]")
{
	class A {
		int foo;
		double bar;
	public:
		A(int baz) : foo(baz), bar() { }
		A(int qux, double fwibble) noexcept : foo(qux), bar(fwibble) {}
	};

	class B {};

	REQUIRE_FALSE(is_constructible_v<A, B>());

	REQUIRE(is_constructible_v<A, int>());
	REQUIRE(is_constructible_v<A, int, double>());
}

using substrate::is_trivially_constructible_v;
TEST_CASE( "[C++ 17] is_trivially_constructible_v helper", "[utility]")
{
	class A {
		int foo;
		double bar;
	public:
		A(int baz) : foo(baz), bar() { }
		A(int qux, double fwibble) noexcept : foo(qux), bar(fwibble) {}
	};

	REQUIRE_FALSE(is_trivially_constructible_v<A, int>());

	REQUIRE(is_trivially_constructible_v<A, const A&>());
}

using substrate::is_nothrow_constructible_v;
TEST_CASE( "[C++ 17] is_nothrow_constructible_v helper", "[utility]")
{
	class A {
		int foo;
		double bar;
	public:
		A(int baz) : foo(baz), bar() { }
		A(int qux, double fwibble) noexcept : foo(qux), bar(fwibble) {}
	};

	REQUIRE_FALSE(is_nothrow_constructible_v<A, int>());

	REQUIRE(is_nothrow_constructible_v<A, int, double>());
}

using substrate::is_unsigned_v;
TEST_CASE( "[C++ 17] is_unsigned_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	enum struct C : int {};
	enum D : unsigned int {};
	enum struct E : unsigned int {};

	REQUIRE_FALSE(is_unsigned_v<A>());
	REQUIRE_FALSE(is_unsigned_v<B>());
	REQUIRE_FALSE(is_unsigned_v<C>());
	REQUIRE_FALSE(is_unsigned_v<D>());
	REQUIRE_FALSE(is_unsigned_v<E>());
	REQUIRE_FALSE(is_unsigned_v<int>());
	REQUIRE_FALSE(is_unsigned_v<signed int>());

	REQUIRE(is_unsigned_v<unsigned int>());
}

using substrate::is_signed_v;
TEST_CASE( "[C++ 17] is_signed_v helper", "[utility]")
{
	class A {};
	enum B : int {};
	enum struct C : int {};
	enum D : unsigned int {};
	enum struct E : unsigned int {};

	REQUIRE_FALSE(is_signed_v<A>());
	REQUIRE_FALSE(is_signed_v<B>());
	REQUIRE_FALSE(is_signed_v<C>());
	REQUIRE_FALSE(is_signed_v<D>());
	REQUIRE_FALSE(is_signed_v<E>());
	REQUIRE_FALSE(is_signed_v<unsigned int>());

	REQUIRE(is_signed_v<int>());
	REQUIRE(is_signed_v<signed int>());
}

using substrate::is_abstract_v;
TEST_CASE( "[C++ 17] is_abstract_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { virtual void bar(); virtual ~B() = default; };
	struct C { virtual void baz() = 0; virtual ~C() = default; };
	struct D : C {};

	REQUIRE_FALSE(is_abstract_v<A>());
	REQUIRE_FALSE(is_abstract_v<B>());

	REQUIRE(is_abstract_v<C>());
	REQUIRE(is_abstract_v<D>());
}

using substrate::is_polymorphic_v;
TEST_CASE( "[C++ 17] is_polymorphic_v helper", "[utility]")
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
TEST_CASE( "[C++ 17] is_empty_v helper", "[utility]")
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
TEST_CASE( "[C++ 17] is_pod_v helper", "[utility]")
{
	struct A { int foo; };
	struct B {
		int bar;
	private:
		int baz;
	};
	struct C { virtual void qux(); virtual ~C() = default;  };

	REQUIRE_FALSE(is_pod_v<B>());
	REQUIRE_FALSE(is_pod_v<C>());

	REQUIRE(is_pod_v<A>());
}

using substrate::is_standard_layout_v;
TEST_CASE( "[C++ 17] is_standard_layout_v helper", "[utility]")
{
	struct A { int foo; };
	struct B {
		int bar;
	private:
		int baz;
	};
	struct C { virtual void qux(); virtual ~C() = default;  };

	REQUIRE_FALSE(is_standard_layout_v<B>());
	REQUIRE_FALSE(is_standard_layout_v<C>());

	REQUIRE(is_standard_layout_v<A>());
}

using substrate::is_trivially_copyable_v;
TEST_CASE( "[C++ 17] is_trivially_copyable_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { B(B const&) {} };
	struct C { virtual void baz(); virtual ~C() = default; };
	struct D { int foo; D(D const&) = default; D(int qux) : foo(qux+1) {} };

	REQUIRE_FALSE(is_trivially_copyable_v<B>());
	REQUIRE_FALSE(is_trivially_copyable_v<C>());

	REQUIRE(is_trivially_copyable_v<A>());
	REQUIRE(is_trivially_copyable_v<D>());
}

using substrate::is_trivial_v;
TEST_CASE( "[C++ 17] is_trivial_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { B() {} };

	REQUIRE_FALSE(is_trivial_v<B>());

	REQUIRE(is_trivial_v<A>());
}

using substrate::is_volatile_v;
TEST_CASE( "[C++ 17] is_volatile_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_volatile_v<A>());
	REQUIRE_FALSE(is_volatile_v<int>());

	REQUIRE(is_volatile_v<volatile int>());
}

using substrate::is_const_v;
TEST_CASE( "[C++ 17] is_const_v helper", "[utility]")
{
	class A {};

	REQUIRE_FALSE(is_const_v<A>());
	REQUIRE_FALSE(is_const_v<int>());
	REQUIRE_FALSE(is_const_v<const int*>());
	REQUIRE_FALSE(is_const_v<const int&>());

	REQUIRE(is_const_v<const int>());
	REQUIRE(is_const_v<int* const>());
	REQUIRE(is_const_v<substrate::remove_reference_t<const int&>>());
}

using substrate::is_literal_type_v;
TEST_CASE( "[C++ 17] is_literal_type_v helper", "[utility]")
{
	struct A { int foo; };
	struct B { virtual ~B(); };

	REQUIRE_FALSE(is_literal_type_v<B>());

	REQUIRE(is_literal_type_v<A>());
}

#endif

/* C++ 14 helpers */
#if __cplusplus < 201402L

using substrate::enable_if_t;
TEST_CASE( "[C++ 14] enable_if_t helper", "[utility]")
{

}

using substrate::underlying_type_t;
TEST_CASE( "[C++ 14] underlying_type_t helper", "[utility]")
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
TEST_CASE( "[C++ 14] conditional_t helper", "[utility]")
{

}

using substrate::remove_reference_t;
TEST_CASE( "[C++ 14] remove_reference_t helper", "[utility]")
{
	REQUIRE(std::is_same<int, remove_reference_t<int>>::value);
	REQUIRE(std::is_same<int, remove_reference_t<int&>>::value);
	REQUIRE(std::is_same<int, remove_reference_t<int&&>>::value);
}

using substrate::remove_cv_t;
TEST_CASE( "[C++ 14] remove_cv_t helper", "[utility]")
{
	using A = const int;
	using B = const int *;
	using C = int *const;
	using D = volatile int;
	using E = volatile int *;
	using F = int* volatile;
	using G = const volatile int;
	using H = const volatile int *;
	using I = int *const volatile;

	REQUIRE(std::is_same<int, remove_cv_t<A>>::value);
	REQUIRE(std::is_same<B, remove_cv_t<B>>::value);
	REQUIRE(std::is_same<int*, remove_cv_t<C>>::value);
	REQUIRE(std::is_same<int, remove_cv_t<D>>::value);
	REQUIRE(std::is_same<E, remove_cv_t<E>>::value);
	REQUIRE(std::is_same<int*, remove_cv_t<F>>::value);
	REQUIRE(std::is_same<int, remove_cv_t<G>>::value);
	REQUIRE(std::is_same<H, remove_cv_t<H>>::value);
	REQUIRE(std::is_same<int*, remove_cv_t<I>>::value);
}

using substrate::remove_const_t;
TEST_CASE( "[C++ 14] remove_const_t helper", "[utility]")
{
	using A = const int;
	using B = const int *;
	using C = int *const;
	using D = volatile int;
	using E = volatile int *;
	using F = int* volatile;
	using G = const volatile int;
	using H = const volatile int *;
	using I = int *const volatile;

	REQUIRE(std::is_same<int, remove_const_t<A>>::value);
	REQUIRE(std::is_same<B, remove_const_t<B>>::value);
	REQUIRE(std::is_same<int*, remove_const_t<C>>::value);
	REQUIRE(std::is_same<D, remove_const_t<D>>::value);
	REQUIRE(std::is_same<E, remove_const_t<E>>::value);
	REQUIRE(std::is_same<F, remove_const_t<F>>::value);
	REQUIRE(std::is_same<volatile int, remove_const_t<G>>::value);
	REQUIRE(std::is_same<H, remove_const_t<H>>::value);
	REQUIRE(std::is_same<int * volatile, remove_const_t<I>>::value);
}

using substrate::remove_volatile_t;
TEST_CASE( "[C++ 14] remove_volatile_t helper", "[utility]")
{
	using A = const int;
	using B = const int *;
	using C = int *const;
	using D = volatile int;
	using E = volatile int *;
	using F = int* volatile;
	using G = const volatile int;
	using H = const volatile int *;
	using I = int *const volatile;

	REQUIRE(std::is_same<A, remove_volatile_t<A>>::value);
	REQUIRE(std::is_same<B, remove_volatile_t<B>>::value);
	REQUIRE(std::is_same<C, remove_volatile_t<C>>::value);
	REQUIRE(std::is_same<int, remove_volatile_t<D>>::value);
	REQUIRE(std::is_same<E, remove_volatile_t<E>>::value);
	REQUIRE(std::is_same<int*, remove_volatile_t<F>>::value);
	REQUIRE(std::is_same<const int, remove_volatile_t<G>>::value);
	REQUIRE(std::is_same<H, remove_volatile_t<H>>::value);
	REQUIRE(std::is_same<int *const, remove_volatile_t<I>>::value);
}

using  substrate::add_cv_t;
TEST_CASE( "[C++14] add_cv_t helper", "[utility]")
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
	REQUIRE(std::is_same<int const* const volatile, add_cv_t<D>>::value);
	REQUIRE(std::is_same<I, add_cv_t<E>>::value);
	REQUIRE(std::is_same<J, add_cv_t<F>>::value);
	REQUIRE(std::is_same<int volatile* const volatile, add_cv_t<G>>::value);
	REQUIRE(std::is_same<I, add_cv_t<H>>::value);
}

using  substrate::add_const_t;
TEST_CASE( "[C++14] add_const_t helper", "[utility]")
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
	REQUIRE(std::is_same<int const* const, add_const_t<D>>::value);
	REQUIRE(std::is_same<int *const, add_const_t<E>>::value);
	REQUIRE(std::is_same<J, add_const_t<F>>::value);
	REQUIRE(std::is_same<int volatile* const, add_const_t<G>>::value);
	REQUIRE(std::is_same<I, add_const_t<H>>::value);
}

using  substrate::add_volatile_t;
TEST_CASE( "[C++14] add_volatile_t helper", "[utility]")
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
	REQUIRE(std::is_same<int * volatile, add_volatile_t<B>>::value);
	REQUIRE(std::is_same<int const volatile, add_volatile_t<C>>::value);
	REQUIRE(std::is_same<int const* volatile, add_volatile_t<D>>::value);
	REQUIRE(std::is_same<I, add_volatile_t<E>>::value);
	REQUIRE(std::is_same<volatile int, add_volatile_t<F>>::value);
	REQUIRE(std::is_same<int volatile* volatile, add_volatile_t<G>>::value);
	REQUIRE(std::is_same<int * volatile, add_volatile_t<H>>::value);
}

using  substrate::add_lvalue_reference_t;
TEST_CASE( "[C++14] add_lvalue_reference_t helper", "[utility]")
{
	REQUIRE(std::is_same<int&, add_lvalue_reference_t<int>>::value);
}

using  substrate::add_rvalue_reference_t;
TEST_CASE( "[C++14] add_rvalue_reference_t helper", "[utility]")
{
	REQUIRE(std::is_same<int&&, add_rvalue_reference_t<int>>::value);
}

using  substrate::remove_pointer_t;
TEST_CASE( "[C++14] remove_pointer_t helper", "[utility]")
{
	REQUIRE(std::is_same<int, remove_pointer_t<int*>>::value);
	REQUIRE(std::is_same<int*, remove_pointer_t<int**>>::value);
}

using  substrate::add_pointer_t;
TEST_CASE( "[C++14] add_pointer_t helper", "[utility]")
{
	REQUIRE(std::is_same<int*, add_pointer_t<int>>::value);
	REQUIRE(std::is_same<int**, add_pointer_t<int*>>::value);
}

using  substrate::make_signed_t;
TEST_CASE( "[C++14] make_signed_t helper", "[utility]")
{
	REQUIRE_FALSE(std::is_same<unsigned int, make_signed_t<int>>::value);
	REQUIRE(std::is_same<int, make_signed_t<int>>::value);
}

using  substrate::make_unsigned_t;
TEST_CASE( "[C++14] make_unsigned_t helper", "[utility]")
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
TEST_CASE("shift nibble", "[utility]") {
	uint64_t value{0x000000000000000FLLU};
	uint64_t value2{0x0123456789ABCDEFLLU};

	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x00000000000000F0LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x0000000000000F00LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x000000000000F000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x00000000000F0000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x0000000000F00000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x000000000F000000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x00000000F0000000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x0000000F00000000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x000000F000000000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x00000F0000000000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x0000F00000000000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x000F000000000000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x00F0000000000000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0x0F00000000000000LLU);
	REQUIRE((value = shift_nibble<decltype(value)>(value, 1U)) == 0xF000000000000000LLU);

	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 2U)) == 0x23456789ABCDEF01LLU);
	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 2U)) == 0x456789ABCDEF0123LLU);
	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 2U)) == 0x6789ABCDEF012345LLU);
	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 2U)) == 0x89ABCDEF01234567LLU);
	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 2U)) == 0xABCDEF0123456789LLU);
	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 2U)) == 0xCDEF0123456789ABLLU);
	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 2U)) == 0xEF0123456789ABCDLLU);
	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 2U)) == 0x0123456789ABCDEFLLU);

	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 4U)) == 0x456789ABCDEF0123LLU);
	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 4U)) == 0x89ABCDEF01234567LLU);
	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 4U)) == 0xCDEF0123456789ABLLU);
	REQUIRE((value2 = shift_nibble<decltype(value2)>(value2, 4U)) == 0x0123456789ABCDEFLLU);
}

using substrate::bswap16;
using substrate::bswap32;
using substrate::bswap64;
TEST_CASE("bswap tests", "[utility]")
{
	SECTION("bswap16")
	{
		REQUIRE(bswap16(0x3412U) == 0x1234U);
		REQUIRE(bswap16(0x1234U) == 0x3412U);
		REQUIRE(bswap16(0x2413U) == 0x1324U);
		REQUIRE(bswap16(0x2143U) == 0x4321U);
	}

	SECTION("bswap32")
	{
		REQUIRE(bswap32(0x78563412U) == 0x12345678U);
		REQUIRE(bswap32(0x12345678U) == 0x78563412U);
		REQUIRE(bswap32(0x34127856U) == 0x56781234U);
		REQUIRE(bswap32(0x56341278U) == 0x78123456U);
	}

	SECTION("bswap64")
	{
		REQUIRE(bswap64(0xEFCDAB8967452301LLU) == 0x0123456789ABCDEFLLU);
		REQUIRE(bswap64(0x0123456789ABCDEFLLU) == 0xEFCDAB8967452301LLU);
		REQUIRE(bswap64(0x89ABCDEF01234567LLU) == 0x67452301EFCDAB89LLU);
		REQUIRE(bswap64(0xAB8967452301EFCDLLU) == 0xCDEF0123456789ABLLU);
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
		std::uniform_int_distribution<int8_t>   int8Dist{std::numeric_limits<int8_t>::min()};
		for(size_t c{}; c < check_itr; ++c)
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
		for(size_t c{}; c < check_itr; ++c)
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
		for(size_t c{}; c < check_itr; ++c)
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
		for(size_t c{}; c < check_itr; ++c)
		{
			auto value = int64Dist(rand);
			auto enc = leb128_encode<int64_t>(value);
			auto res = leb128_decode<int64_t>(enc);
			REQUIRE(value == res);
		}
	}

	SECTION("unsigned leb128 uint8_t")
	{
		std::uniform_int_distribution<uint8_t>   uint8Dist{std::numeric_limits<uint8_t>::min()};
		for(size_t c{}; c < check_itr; ++c)
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
		for(size_t c{}; c < check_itr; ++c)
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
		for(size_t c{}; c < check_itr; ++c)
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
		for(size_t c{}; c < check_itr; ++c)
		{
			auto value = uint64Dist(rand);
			auto enc = leb128_encode<uint64_t>(value);
			auto res = leb128_decode<uint64_t>(enc);
			REQUIRE(value == res);
		}
	}
}
