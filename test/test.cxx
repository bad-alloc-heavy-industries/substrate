#ifdef _WINDOWS
#include <cstdlib>
#include <crtdbg.h>
#endif
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#ifdef _WINDOWS
void invalidHandler(const wchar_t *, const wchar_t *, const wchar_t *, const uint32_t, const uintptr_t) { }
#endif

int main(int argCount, char **argList)
{
#ifdef _WINDOWS	
	_set_invalid_parameter_handler(invalidHandler);
	_CrtSetReportMode(_CRT_ASSERT, 0);
	_CrtSetReportMode(_CRT_ERROR, 0);	
#endif

	return Catch::Session().run(argCount, argList);
}
