#include <thread>
#include <chrono>
#include <future>

#include <substrate/latch>
#include <substrate/threaded_queue>

#include <catch2/catch.hpp>

TEST_CASE("empty", "[threadedQueue_t]")
{
	substrate::threadedQueue_t<std::int32_t> queue;
	REQUIRE(queue.empty());
	REQUIRE(queue.size() == 0);
}

TEST_CASE("emplace", "[threadedQueue_t]")
{
	substrate::threadedQueue_t<std::int32_t> queue;
	queue.emplace(5);
	REQUIRE(!queue.empty());
	REQUIRE(queue.size() == 1);
}

TEST_CASE("push", "[threadedQueue_t]")
{
	substrate::threadedQueue_t<std::int32_t> queue;
	queue.push(5);
	queue.push(10);
	REQUIRE(!queue.empty());
	REQUIRE(queue.size() == 2);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
TEST_CASE("pop", "[threadedQueue_t]")
{
	substrate::threadedQueue_t<std::int32_t> queue;
	queue.push(5);
	queue.push(10);
	queue.emplace(15);
	REQUIRE(!queue.empty());
	REQUIRE(queue.size() == 3);
	REQUIRE(queue.pop() == 5);
	REQUIRE(!queue.empty());
	REQUIRE(queue.size() == 2);
	REQUIRE(queue.pop() == 10);
	REQUIRE(queue.pop() == 15);
	REQUIRE(queue.empty());
	REQUIRE(queue.size() == 0);
}

TEST_CASE("async", "[threadedQueue_t]")
{
	substrate::latch_t barrier{2};
	substrate::threadedQueue_t<std::int32_t> queue;
	REQUIRE(queue.empty());
	REQUIRE(queue.size() ==0);

	auto result = std::async(std::launch::async, [&]() noexcept -> std::int32_t
	{
		barrier.arriveAndWait();
		return queue.pop();
	});
	barrier.arriveAndWait();
	std::this_thread::sleep_for(std::chrono::microseconds(25));

	queue.push(-100);
	REQUIRE(result.get() == -100);
	REQUIRE(queue.empty());
	REQUIRE(queue.size() == 0);
}
