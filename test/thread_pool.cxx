// SPDX-License-Identifier: BSD-3-Clause

#include <chrono>
#include <condition_variable>
#include <mutex>

#include <substrate/affinity>
#include <substrate/utility>
#include <substrate/thread_pool>

#include <catch2/catch.hpp>

namespace
{
std::mutex workMutex;
std::condition_variable workCond;

constexpr std::size_t totalLoopIterations{1000000U};

SUBSTRATE_NOINLINE bool dummyWork()
{
	std::this_thread::sleep_for(std::chrono::microseconds(100));
	return true;
}

SUBSTRATE_NOINLINE bool busyWork(const std::size_t iterations)
{
	volatile std::size_t counter{};
	for (size_t i{}; i < iterations; ++i)
	{
		for (size_t j{}; j < totalLoopIterations; ++j)
			++counter;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(25));
	workCond.notify_all();
	return counter == size_t(iterations * totalLoopIterations);
}
} // namespace

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("unused", "[threadPool_t]")
{
	substrate::affinity_t affinity{};
	auto pool{substrate::make_unique_nothrow<substrate::threadPool_t<decltype(dummyWork)>>(dummyWork)};
	REQUIRE(pool);
	REQUIRE(pool->valid());
	REQUIRE(pool->numProcessors() == affinity.numProcessors());
	REQUIRE(!pool->finish());
	REQUIRE(!pool->valid());
	REQUIRE(!pool->finish());
	REQUIRE(pool->numProcessors() == affinity.numProcessors());
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("once", "[threadPool_t]")
{
	auto pool{substrate::make_unique_nothrow<substrate::threadPool_t<decltype(dummyWork)>>(dummyWork)};
	REQUIRE(pool);
	REQUIRE(pool->valid());
	REQUIRE(pool->ready());
	REQUIRE(!pool->queue());
	REQUIRE(pool->valid());
	REQUIRE(pool->finish());
	REQUIRE(!pool->valid());
	REQUIRE(!pool->finish());
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("queue wait", "[threadPool_t]")
{
	substrate::threadPool_t<bool(size_t)> pool{busyWork};
	REQUIRE(pool.valid());
	REQUIRE(pool.ready());
	const auto threads{pool.numProcessors()};
	REQUIRE(threads != 0);
	// burst queue
	for (std::size_t i{}; i < threads; ++i)
		SUBSTRATE_NOWARN_UNUSED(const auto result) = pool.queue(threads - i);
	//REQUIRE(!pool.ready());
	// queue threads+1
	SUBSTRATE_NOWARN_UNUSED(const auto result) = pool.queue(threads);
	// queue after work completions
	[]() noexcept
	{
		std::unique_lock<std::mutex> lock{workMutex};
		workCond.wait(lock);
	}();
	REQUIRE(pool.queue(threads));
	// finish
	REQUIRE(pool.finish());
	// invalid
	REQUIRE(!pool.valid());
	// already finished
	REQUIRE(!pool.finish());
}
