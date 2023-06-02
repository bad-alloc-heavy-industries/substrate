// SPDX-License-Identifier: BSD-3-Clause

#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#endif
#ifndef _MSC_VER
#include <cxxabi.h>
#endif

#include <string>
#include <system_error>
#include <vector>

#include "substrate/utility"

namespace substrate
{
	std::string decode_typename(const char *const mangledName)
	{
#ifndef _MSC_VER
		int status{};
		auto *const demangler{abi::__cxa_demangle(mangledName, nullptr, nullptr, &status)};
		if (status == -1)
			throw std::runtime_error("A memory allocation failure occurred");
		else if (status == -3)
			return {mangledName};
		else if (status == 0)
		{
			std::unique_ptr<char, void (*)(void *)> _demangle{demangler, std::free};
			return {_demangle ? _demangle.get() : mangledName};
		}
		// status == 2 -- fallthrough
#endif
#ifdef _WIN32
		std::vector<char> buffer(2048U);
		const auto result{UnDecorateSymbolName(mangledName, buffer.data(), 2048U, UNDNAME_COMPLETE)};
		if (result != 0)
			return {buffer.data()};
		else
			throw std::system_error(static_cast<int>(GetLastError()), std::system_category());
#endif
		return mangledName;
	}
} // namespace substrate
