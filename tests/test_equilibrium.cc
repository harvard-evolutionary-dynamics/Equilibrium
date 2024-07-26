#include <iostream>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <equilibrium/simulation.h>
#include <equilibrium/graph.h>

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

TEST_CASE("star graph", "[StarGraph]") {
  const auto star = equilibrium::StarGraph(4);
  REQUIRE(star.N == 4);
  REQUIRE(star.name == "star");

  const std::vector<std::vector<int>> expected = {{1, 2, 3}, {0}, {0}, {0}};
  REQUIRE(star.adjacency_list == expected);
}

TEST_CASE("cycle graph", "[CycleGraph]") {
  const auto cycle = equilibrium::CycleGraph(4);
  REQUIRE(cycle.N == 4);
  REQUIRE(cycle.name == "cycle");

  const std::vector<std::vector<int>> expected = {{1, 3}, {0, 2}, {1, 3}, {2, 0}};
  REQUIRE(cycle.adjacency_list == expected);
}

TEST_CASE("complete graph", "[CompleteGraph]") {
  const auto complete = equilibrium::CompleteGraph(4);
  REQUIRE(complete.N == 4);
  REQUIRE(complete.name == "complete");

  const std::vector<std::vector<int>> expected = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};
  REQUIRE(complete.adjacency_list == expected);
}

TEST_CASE("double star N=1", "[DoubleStarGraph]") {
  const auto double_star = equilibrium::DoubleStarGraph(1);
  REQUIRE(double_star.N == 1);
  REQUIRE(double_star.name == "double star");

  std::vector<std::vector<int>> expected;
  expected.resize(1);
  REQUIRE(double_star.adjacency_list == expected);
}

TEST_CASE("double star N=2", "[DoubleStarGraph]") {
  const auto double_star = equilibrium::DoubleStarGraph(2);
  REQUIRE(double_star.N == 2);
  REQUIRE(double_star.name == "double star");

  std::vector<std::vector<int>> expected = {{1}, {0}};
  REQUIRE(double_star.adjacency_list == expected);
}

TEST_CASE("double star N=3", "[DoubleStarGraph]") {
  const auto double_star = equilibrium::DoubleStarGraph(3);
  REQUIRE(double_star.N == 3);
  REQUIRE(double_star.name == "double star");

  std::vector<std::vector<int>> expected = {{1, 2}, {0}, {0}};
  REQUIRE(double_star.adjacency_list == expected);
}

TEST_CASE("double star N=4", "[DoubleStarGraph]") {
  const auto double_star = equilibrium::DoubleStarGraph(4);
  REQUIRE(double_star.N == 4);
  REQUIRE(double_star.name == "double star");

  std::vector<std::vector<int>> expected = {{1, 2}, {0, 3}, {0}, {1}};
  REQUIRE(double_star.adjacency_list == expected);
}

TEST_CASE("double star N=5", "[DoubleStarGraph]") {
  const auto double_star = equilibrium::DoubleStarGraph(5);
  REQUIRE(double_star.N == 5);
  REQUIRE(double_star.name == "double star");

  std::vector<std::vector<int>> expected = {{1, 2, 4}, {0, 3}, {0}, {1}, {0}};
  REQUIRE(double_star.adjacency_list == expected);
}

