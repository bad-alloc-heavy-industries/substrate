// SPDX-License-Identifier: BSD-3-Clause
#ifdef _WINDOWS
#include <cstdlib>
#include <crtdbg.h>
#endif
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include "substrate/console"

using substrate::console;

#ifdef _WINDOWS
void invalidHandler(const wchar_t *, const wchar_t *, const wchar_t *, const uint32_t, const uintptr_t) { }
#endif

int main(int argCount, char **argList) noexcept
{
#ifdef _WINDOWS
	_set_invalid_parameter_handler(invalidHandler);
	_CrtSetReportMode(_CRT_ASSERT, 0);
	_CrtSetReportMode(_CRT_ERROR, 0);
#endif

	console = {stdout, stderr};

	try
	{
		return Catch::Session().run(argCount, argList);
	}
	catch (const std::domain_error &error)
	{
		console.error("Error running test suite: ", error.what());
		return 2;
	}
}
/* vim: set ft=cpp ts=4 sw=4 noexpandtab: */
