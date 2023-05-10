// SPDX-License-Identifier: BSD-3-Clause
#include <substrate/command_line/arguments>
#include <catch2/catch.hpp>

using namespace std::literals::string_view_literals;
using namespace substrate::commandLine;

TEST_CASE("parseArguments invalid invocations", "[command_line::parseArguments]")
{
	constexpr std::array<const char *, 1U> dummyArgs{{nullptr}};
	constexpr static options_t dummyOptions{};

	REQUIRE(parseArguments(0U, dummyArgs.data(), dummyOptions) == std::nullopt);
	REQUIRE(parseArguments(dummyArgs.size(), nullptr, dummyOptions) == std::nullopt);
	REQUIRE(parseArguments(static_cast<size_t>(-1), dummyArgs.data(), dummyOptions) == std::nullopt);
}
