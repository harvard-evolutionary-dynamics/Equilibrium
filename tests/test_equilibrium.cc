#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <equilibrium/simulation.h>

TEST_CASE("logic", "[basic]") {
  REQUIRE(1 + 1 == 2);
}

TEST_CASE("3 types", "[NumberOfTypes]") {
  REQUIRE(equilibrium::NumberOfTypes({1, 2, 3}) == 3);
}

TEST_CASE("1 type", "[NumberOfTypes]") {
  REQUIRE(equilibrium::NumberOfTypes({1, 1, 1}) == 1);
}

TEST_CASE("2 types", "[NumberOfTypes]") {
  REQUIRE(equilibrium::NumberOfTypes({1, 2, 1}) == 2);
}

TEST_CASE("0 types", "[NumberOfTypes]") {
  REQUIRE(equilibrium::NumberOfTypes({}) == 0);
}
