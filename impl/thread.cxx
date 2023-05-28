// SPDX-License-Identifier: BSD-3-Clause
#include <array>
#include <system_error>
#include <stdexcept>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif
#if !defined(_WIN32) || defined(__WINPTHREADS_VERSION)
// MSYS2 + winpthreads need both APIs to translate between native_handle_type
// (pthread) and HANDLE (native).
#include <unistd.h>
#include <pthread.h>
#endif
#if defined(_POSIX_THREADS) && defined(__APPLE__)
#include <mach/mach.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#elif defined(_POSIX_THREADS) && defined(_GNU_SOURCE)
#include <sched.h>
#endif // _POSIX_THREADS && _GNU_SOURCE
#if (defined(__x86_64__) || defined(__i386__)) && !defined(_WIN32)
#include <cpuid.h>
#endif

#include "substrate/thread"

namespace substrate
{
	namespace thread
	{
		std::thread::native_handle_type currentThread()
		{
#ifdef _POSIX_THREADS
			return pthread_self();
#elif defined(_WIN32)
			return GetCurrentThread();
#else
			throw std::runtime_error("not implemented");
#endif
		}

		uint32_t currentCPU()
		{
#if defined(_POSIX_THREADS) && defined(_GNU_SOURCE)
			const auto cpu{sched_getcpu()};
			if (cpu == -1)
				throw std::system_error{errno, std::system_category()};
#elif defined(__APPLE__)
#if defined(__arm__)
			// Apple XNU stores the CPU id in the lowest three bits of
			// register 13 of CP15.
			// MRC coproc, opcode1, Rd, CRn, CRm{, opcode2}
			uint32_t result{};
			__asm__("mrc p15, 0, %[p], c13, c0, 3" : [p] "=&r"(result));
			return result & 0x3;
#elif defined(__arm64__) || defined(__aarch64__)
			// In this case, they're a good citizen.
			// https://developer.arm.com/documentation/ddi0601/2020-12/AArch64-Registers/TPIDRRO-EL0--EL0-Read-Only-Software-Thread-ID-Register
			uint64_t result{};
			__asm__("mrs %0, TPIDRRO_EL0" : "=r"(result));
			return static_cast<uint32_t>(result & 0xFFF);
#elif defined(__x86_64__) || defined(__i386__)
			// Officially, they want you to access the IDT register, and that's
			// most likely protected by UPIC:
			// https://en.wikichip.org/wiki/x86/umip
			// So let's try the alternative, CPUID then accessing the APIC ID.
			unsigned int eax{};
			unsigned int ebx{};
			unsigned int ecx{};
			unsigned int edx{};
			const int function{0x1};
			const auto result{__get_cpuid(function, &eax, &ebx, &ecx, &edx)};
			if (result)
				return (ebx >> 24U) & 0xFF;
			throw std::runtime_error("error when retrieving the CPUID");
#else
			throw std::runtime_error("not implemented");
#endif
#elif defined(_WIN32)
			return GetCurrentProcessorNumber();
#else
			throw std::runtime_error("not implemented");
#endif
		}
	} // namespace thread
} // namespace substrate
