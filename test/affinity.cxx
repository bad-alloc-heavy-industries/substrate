// SPDX-License-Identifier: BSD-3-Clause

#include <future>

#include <random>
#include <substrate/utility>
#include <substrate/affinity>
#include <substrate/fixed_vector>

#ifdef _WIN32 
#define NOMINMAX
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

#include <catch2/catch.hpp>

#if defined(_WIN32)
using processorVector_t = substrate::fixedVector_t<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>;
using processorIterator_t = substrate::boundedIterator_t<const SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>;

static processorVector_t retrieveProcessorInfo() noexcept
{
	processorVector_t processorInfo{};
	DWORD returnLength{};

	while (!GetLogicalProcessorInformationEx(RelationGroup, processorInfo.data(), &returnLength))
	{
		const auto error{static_cast<uint32_t>(GetLastError())};
		REQUIRE(error == ERROR_INSUFFICIENT_BUFFER);
		constexpr auto structLength{sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)};
		// Compute how many structs worth of information the kernel has to return..
		auto count{returnLength / structLength};
		count += returnLength - (count * structLength) ? 1 : 0;
		// Then allocate a fixedVector_t large enough for that.
		processorInfo = {count};
	}
	return processorInfo;
}

static void nextProcessor(const processorVector_t &processorInfo,
	processorIterator_t &info, uint16_t &groupIndex, uint8_t &maskOffset)
{
	while (info != processorInfo.end())
	{
		while (info->Relationship != RelationGroup)
		{
			++info;
			REQUIRE(info != processorInfo.end());
		}
		auto processor{*info};
		REQUIRE(groupIndex < processor.Group.ActiveGroupCount);
		for (; groupIndex < processor.Group.ActiveGroupCount; ++groupIndex)
		{
			const auto &group{processor.Group.GroupInfo[groupIndex]};
			for (; maskOffset < sizeof(KAFFINITY) * 8; ++maskOffset)
			{
				const auto mask{group.ActiveProcessorMask >> maskOffset};
				if (mask & 1)
					return;
			}
			maskOffset = 0;
		}
		++info;
	}
	throw std::runtime_error("Could not find another valid group:processor combo while there are entries left in affinity_t");
}
#elif defined(__APPLE__)
static int sched_getcpu() noexcept
{
	boolean_t getDefault{false};
	thread_affinity_policy_data_t policy{};
	mach_msg_type_number_t numPolicies{1};
	const thread_port_t machThread{mach_thread_self()};
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	REQUIRE(thread_policy_get(machThread, THREAD_AFFINITY_POLICY, reinterpret_cast<thread_policy_t>(&policy), &numPolicies, &getDefault) == 0);
	return policy.affinity_tag;
}
#endif

TEST_CASE("construct", "[affinity_t]")
{
	const auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>()};
	REQUIRE(affinity);
}

TEST_CASE("processor count", "[affinity_t]")
{
	const auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>()};
	REQUIRE(affinity);
	REQUIRE(affinity->numProcessors() == std::thread::hardware_concurrency());
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("iteration", "[affinity_t]")
{
	const auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>()};
	REQUIRE(affinity);
#if defined(_POSIX_THREADS) && defined(_GNU_SOURCE)
	cpu_set_t affinitySet{};
	REQUIRE(sched_getaffinity(0, sizeof(cpu_set_t), &affinitySet) == 0);

	REQUIRE(affinity->begin() != affinity->end());
	std::size_t count{0};
	for (const auto processor : *affinity)
	{
		++count;
		REQUIRE(CPU_ISSET(processor, &affinitySet));
	}
	REQUIRE(count == affinity->numProcessors());
#elif defined(_POSIX_THREADS) && defined(__APPLE__)
	boolean_t getDefault{false};
	thread_affinity_policy_data_t policy{};
	mach_msg_type_number_t numPolicies{1};
	const thread_port_t machThread{mach_thread_self()};
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	REQUIRE(thread_policy_get(machThread, THREAD_AFFINITY_POLICY, reinterpret_cast<thread_policy_t>(&policy), &numPolicies, &getDefault) == 0);
	REQUIRE(affinity->begin() != affinity->end());
	std::size_t count{0};
	for (const auto processor: *affinity)
	{
		(void)processor;
		++count;
		const bool isCorePinned = policy.affinity_tag & (1 << count);
		const bool canUseAnyCores = !policy.affinity_tag;
		REQUIRE((isCorePinned || canUseAnyCores));
	}
	REQUIRE(count == affinity->numProcessors());
#elif defined(_WIN32)
	const auto procCount{GetActiveProcessorCount(ALL_PROCESSOR_GROUPS)};
	REQUIRE(procCount > 0);
	REQUIRE(affinity->numProcessors() == procCount);
#endif
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("pinning", "[affinity_t]")
{
	const auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>()};
	REQUIRE(affinity);
#if defined(_POSIX_THREADS) && !defined(__WINPTHREADS_VERSION)
	for (const auto &processor : *affinity)
	{
		const auto result = std::async(std::launch::async, [&](const uint32_t currentProcessor) noexcept -> int32_t
		{
			try
				{ affinity->pinThreadTo(currentProcessor); }
			catch (const std::out_of_range &)
				{ return INT32_MAX; }
			return sched_getcpu();
		}, processor).get();
		REQUIRE(result != -1);
		REQUIRE(result == static_cast<int32_t>(processor));
	}
#elif defined(_WIN32)
	std::size_t count{0};
	for (const auto &processor : *affinity)
	{
		const auto result = std::async(std::launch::async,
			[&](const uint32_t currentProcessor) noexcept -> GROUP_AFFINITY
			{
				affinity->pinThreadTo(currentProcessor);
				GROUP_AFFINITY res{};
				REQUIRE(GetThreadGroupAffinity(GetCurrentThread(), &res));
				return res;
			}, count++
		).get();
		REQUIRE(result.Mask == UINT64_C(1) << processor.second);
		REQUIRE(result.Group == processor.first);
	}
#endif

	REQUIRE(std::async(std::launch::async, [&]() noexcept -> bool
	{
		try { affinity->pinThreadTo(affinity->numProcessors()); }
		catch (const std::out_of_range &) { return true; }
		return false;
	}).get());
}

TEST_CASE("thread cap", "[affinity_t]")
{
#if defined(_POSIX_THREADS) && !defined(__WINPTHREADS_VERSION)
#if defined(__APPLE__)
	const auto processor { static_cast<uint32_t>(sched_getcpu()) };
#else
	const auto processor
	{
		[&]() noexcept -> uint32_t
		{
			cpu_set_t affinitySet{};
			REQUIRE(sched_getaffinity(0, sizeof(cpu_set_t), &affinitySet) == 0);
			for (uint32_t i{0}; i < CPU_SETSIZE; ++i)
			{
				if (CPU_ISSET(i, &affinitySet))
					return i;
			}
			static_assert(CPU_SETSIZE < UINT32_MAX);
			return UINT32_MAX;
		}()
	};
#endif // defined(__APPLE__)

	REQUIRE(processor != UINT32_MAX);
	auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>(1U)};

	REQUIRE(affinity);
	REQUIRE(affinity->numProcessors() == 1);
	REQUIRE(affinity->begin() != affinity->end());
	REQUIRE(*affinity->begin() == processor);
#elif defined(_WIN32)
	const auto processor
	{
		[&]() -> std::pair<uint16_t, uint8_t>
		{
			const auto processorInfo{retrieveProcessorInfo()};
			auto info{processorInfo.begin()};
			uint16_t groupIndex{};
			uint8_t maskOffset{};
			nextProcessor(processorInfo, info, groupIndex, maskOffset);
			return {groupIndex, maskOffset};
		}()
	};

	auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>(1U)};

	REQUIRE(affinity);
	REQUIRE(affinity->numProcessors() == 1);
	REQUIRE(affinity->begin() != affinity->end());
	REQUIRE(affinity->begin()->first == processor.first);
	REQUIRE(affinity->begin()->second == processor.second);
#endif
}

TEST_CASE("user pinning", "[affinity_t]")
{
#if defined(_POSIX_THREADS) && defined(_GNU_SOURCE)
	const auto processor
	{
		[&]() -> uint32_t
		{
			cpu_set_t affinitySet{};
			REQUIRE(sched_getaffinity(0, sizeof(cpu_set_t), &affinitySet) == 0);
			for (uint32_t i{0}; i < CPU_SETSIZE; ++i)
			{
				if (CPU_ISSET(i, &affinitySet))
					return i;
			}
			static_assert(CPU_SETSIZE < UINT32_MAX);
			return UINT32_MAX;
		}()
	};
	const std::initializer_list<uint32_t> processorList {processor};

	REQUIRE(processor != UINT32_MAX);
	auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>(0U, processorList)};

	REQUIRE(affinity);
	REQUIRE(affinity->numProcessors() == 1);
	REQUIRE(affinity->begin() != affinity->end());
	REQUIRE(*affinity->begin() == processor);
#elif defined(_POSIX_THREADS) && defined(__APPLE__)
	std::default_random_engine gen{std::random_device{}()};
	std::uniform_int_distribution<uint32_t> distrib{0U, std::thread::hardware_concurrency() - 1U};
	const uint32_t processor {distrib(gen)};
	const std::initializer_list<uint32_t> processorList {processor};

	REQUIRE(processor != UINT32_MAX);

	auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>(0U, processorList)};

	REQUIRE(affinity);
	REQUIRE(affinity->numProcessors() == 1);
	REQUIRE(affinity->begin() != affinity->end());
	REQUIRE(*affinity->begin() == processor);
#elif defined(_WIN32)
	const auto processor
	{
		[&]() -> std::pair<uint16_t, uint8_t>
		{
			const auto processorInfo{retrieveProcessorInfo()};
			auto info{processorInfo.begin()};
			uint16_t groupIndex{};
			uint8_t maskOffset{};
			nextProcessor(processorInfo, info, groupIndex, maskOffset);
			return {groupIndex, maskOffset};
		}()
	};
	const std::initializer_list<uint32_t> processorList {0U};

	auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>(0U, processorList)};

	REQUIRE(affinity);
	REQUIRE(affinity->numProcessors() == 1);
	REQUIRE(affinity->begin() != affinity->end());
	REQUIRE(affinity->begin()->first == processor.first);
	REQUIRE(affinity->begin()->second == processor.second);
#endif
}

TEST_CASE("pin second core", "[affinity_t]")
{
#if defined(_POSIX_THREADS) && defined(_GNU_SOURCE)
	const auto processor
		{
			[&]() -> uint32_t
			{
				cpu_set_t affinitySet{};
				REQUIRE(sched_getaffinity(0, sizeof(cpu_set_t), &affinitySet) == 0);
				if (CPU_COUNT(&affinitySet) < 2)
					throw std::runtime_error("This test can only be run on a multi-core machine with at "
						"least 2 cores in the allowed affinity set");
				bool ready{false};
				for (uint32_t i{0}; i < CPU_SETSIZE; ++i)
				{
					if (CPU_ISSET(i, &affinitySet))
					{
						if (ready)
							return i;
						CPU_CLR(i, &affinitySet);
						ready = true;
					}
				}
				static_assert(CPU_SETSIZE < UINT32_MAX);
				return UINT32_MAX;
			}()
		};
		const std::initializer_list<uint32_t> processorList {uint32_t(processor)};

		REQUIRE(processor != UINT32_MAX);
		auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>(0U, processorList)};

		REQUIRE(affinity);
		REQUIRE(affinity->numProcessors() == 1);
		REQUIRE(affinity->begin() != affinity->end());
		REQUIRE(*affinity->begin() == processor);
#elif defined(_POSIX_THREADS) && defined(__APPLE__)
	const auto processor
	{
		[&]() -> uint32_t
		{
			const auto numThreads = std::thread::hardware_concurrency();
			if (numThreads < 2)
				throw std::runtime_error("This test can only be run on a multi-core machine with at "
					"least 2 cores in the allowed affinity set");
			return numThreads - 1;
		}()
	};
	const std::initializer_list<uint32_t> processorList {uint32_t(processor)};

	REQUIRE(processor != UINT32_MAX);

	auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>(0U, processorList)};

	REQUIRE(affinity);
	REQUIRE(affinity->numProcessors() == 1);
	REQUIRE(affinity->begin() != affinity->end());
	REQUIRE(*affinity->begin() == processor);
#elif defined(_WIN32)
	const auto processor
	{
		[&]() -> std::pair<uint16_t, uint8_t>
		{
			if (GetActiveProcessorCount(ALL_PROCESSOR_GROUPS) < 2)
				throw std::runtime_error("This test can only be run on a multi-core machine with at "
					"least 2 cores in the allowed affinity set");
			const auto processorInfo{retrieveProcessorInfo()};
			auto info{processorInfo.begin()};
			uint16_t groupIndex{};
			uint8_t maskOffset{};
			nextProcessor(processorInfo, info, groupIndex, maskOffset);
			++maskOffset;
			nextProcessor(processorInfo, info, groupIndex, maskOffset);
			return {groupIndex, maskOffset};
		}()
	};
	const std::initializer_list<uint32_t> processorList {uint32_t(1)};

	auto affinity{substrate::make_unique_nothrow<substrate::affinity_t>(0U, processorList)};

	REQUIRE(affinity);
	REQUIRE(affinity->numProcessors() == 1);
	REQUIRE(affinity->begin() != affinity->end());
	REQUIRE(affinity->begin()->first == processor.first);
	REQUIRE(affinity->begin()->second == processor.second);
#endif
}
