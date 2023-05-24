// SPDX-License-Identifier: BSD-3-Clause

#include <algorithm>
#include <system_error>
#include <stdexcept>
#include <thread>

#ifdef _WIN32 
#define WIN32_LEAN_AND_MEAN
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
#endif

#include "substrate/affinity"
#include "substrate/fixed_vector"

namespace substrate
{
#ifdef _WIN32
using processorInfo_t = fixedVector_t<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>;
static constexpr uint8_t KAFFINITY_BITS = sizeof(KAFFINITY) * 8;

SUBSTRATE_NO_DISCARD(static processorInfo_t retrieveProcessorInfo())
{
	processorInfo_t processorInfo{};
	DWORD returnLength{};

	while (!GetLogicalProcessorInformationEx(RelationGroup, processorInfo.data(), &returnLength))
	{
		const auto error{GetLastError()};
		if (error == ERROR_INSUFFICIENT_BUFFER)
		{
			constexpr auto structLength{sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)};
			// Compute how many structs worth of information the kernel has to return..
			auto count{returnLength / structLength};
			count += returnLength - (count * structLength) ? 1 : 0;
			// Then allocate a fixedVector_t large enough for that.
			processorInfo = {count};
		}
		else
			throw std::system_error{static_cast<int>(error), std::system_category()};
	}
	return processorInfo;
}
#elif defined(_POSIX_THREADS) && defined(__APPLE__)
SUBSTRATE_NO_DISCARD(static unsigned int hardwareConcurrencyFromSysctl())
{
	unsigned int activeCpuCount{};
	size_t len{sizeof(activeCpuCount)};
	const auto result{sysctlbyname("hw.activecpu", &activeCpuCount, &len, nullptr, 0)};
	if (result != 0)
		throw std::system_error{result, std::system_category()};
	return activeCpuCount;
}
#endif

affinity_t::affinity_t(std::size_t threadCount, const std::vector<uint32_t>& pinning)
	: processors{}
{
#if defined(_POSIX_THREADS) && defined(_GNU_SOURCE)
	cpu_set_t affinity{};
	if (sched_getaffinity(0, sizeof(cpu_set_t), &affinity) != 0)
		throw std::system_error{errno, std::system_category()};
	for (uint32_t i{}; i < CPU_SETSIZE; ++i)
	{
		if (CPU_ISSET(i, &affinity))
		{
			if ((!threadCount || processors.size() < threadCount) &&
			(pinning.empty() || std::find(pinning.cbegin(), pinning.cend(), i) != pinning.cend()))
				processors.emplace_back(i);
			CPU_CLR(i, &affinity);
		}
		else if (!CPU_COUNT(&affinity))
			break;
	}
#elif defined(_POSIX_THREADS) && defined(__APPLE__)
	const auto coreCount
	{
		[]() -> unsigned int
		{
			auto result = std::thread::hardware_concurrency();
			if (result > 0)
				return result;
			return hardwareConcurrencyFromSysctl();
		}()
	};
	for (uint32_t i{}; i < coreCount; ++i)
	{
		if ((!threadCount || processors.size() < threadCount) &&
			(pinning.empty() || std::find(pinning.cbegin(), pinning.cend(), i) != pinning.cend()))
			processors.emplace_back(i);
	}
#elif defined(_WIN32)
	const auto processorInfo{retrieveProcessorInfo()};
	size_t count{};

	for (const auto &processor : processorInfo)
	{
		if (processor.Relationship != RelationGroup)
			continue;
		// NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
		for (uint16_t groupIndex{}; groupIndex < processor.Group.ActiveGroupCount; ++groupIndex)
		{
			// NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access,cppcoreguidelines-pro-bounds-constant-array-index)
			const auto &group{processor.Group.GroupInfo[groupIndex]};
			auto mask{group.ActiveProcessorMask};
			for (uint8_t i{}; i < KAFFINITY_BITS; ++i)
			{
				if (mask & 1)
				{
					if ((!threadCount || processors.size() < threadCount) &&
						(pinning.empty() || std::find(pinning.cbegin(), pinning.cend(), count) != pinning.cend()))
						processors.emplace_back(groupIndex, i);
					++count;
				}
				mask >>= 1;
				if (!mask)
					break;
			}
		}
	}
#else
	(void)threadCount;
	(void)pinning;
	throw std::runtime_error("Not implemented");
#endif
}

void affinity_t::pinTo(std::thread::native_handle_type thread, std::size_t index) const
{
	if (index >= processors.size())
		throw std::out_of_range{"index into thread affinity object too large"};
#if defined(_POSIX_THREADS) && defined(_GNU_SOURCE)
	cpu_set_t affinity{};
	CPU_SET(processors[index], &affinity);
	pthread_setaffinity_np(thread, sizeof(cpu_set_t), &affinity);
#elif defined(_POSIX_THREADS) && defined(__APPLE__)
	const auto affinity = processors[index]; 
	thread_affinity_policy_data_t policy = { static_cast<integer_t>(affinity) };
	const thread_port_t machThread{pthread_mach_thread_np(thread)};
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	thread_policy_set(machThread, THREAD_AFFINITY_POLICY, reinterpret_cast<thread_policy_t>(&policy), 1);
#elif defined(_WIN32)
	uint16_t group{};
	uint8_t processor{};
	std::tie(group, processor) = processors[index];
	const auto groupMask = KAFFINITY(UINT64_C(1) << processor);
	const GROUP_AFFINITY affinity{groupMask, group, {}};
	#if defined(__WINPTHREADS_VERSION)
		HANDLE nativeThread{pthread_gethandle(thread)};
	#else
		HANDLE nativeThread{thread};
	#endif
	SetThreadGroupAffinity(nativeThread, &affinity, nullptr);
#else
	(void)thread;
	throw std::runtime_error("not implemented");
#endif
}

void affinity_t::pinThreadTo(const std::size_t index) const
{
#ifdef _POSIX_THREADS
	pinTo(pthread_self(), index);
#elif defined(_WIN32)
	pinTo(GetCurrentThread(), index);
#else
	(void)index;
	throw std::runtime_error("not implemented");
#endif
}
} // namespace substrate
