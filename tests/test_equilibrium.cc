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

TEST_CASE("fully diverse", "[NumberOfUnmatchingPairs]") {
  REQUIRE(equilibrium::NumberOfUnmatchingPairs({0, 1, 2}) == 3);
}

TEST_CASE("fully homogeneous", "[NumberOfUnmatchingPairs]") {
  REQUIRE(equilibrium::NumberOfUnmatchingPairs({2, 2, 2}) == 0);
}

TEST_CASE("mixed population", "[NumberOfUnmatchingPairs]") {
  REQUIRE(equilibrium::NumberOfUnmatchingPairs({2, 1, 2, 0, 2}) == 7);
}

TEST_CASE("N=1", "[NumberOfUnmatchingPairs]") {
  REQUIRE(equilibrium::NumberOfUnmatchingPairs({2}) == 0);
}

TEST_CASE("fully diverse links", "[NumberOfUnmatchingLinks]") {
  const equilibrium::Graph graph{3, "", {{1}, {2, 0}, {1}}};
  REQUIRE(equilibrium::NumberOfUnmatchingLinks({0, 1, 2}, graph) == 2*2);
}

TEST_CASE("fully homogeneous links", "[NumberOfUnmatchingLinks]") {
  const equilibrium::Graph graph{3, "", {{1}, {2, 0}, {1}}};
  REQUIRE(equilibrium::NumberOfUnmatchingLinks({2, 2, 2}, graph) == 0);
}

TEST_CASE("mixed population links", "[NumberOfUnmatchingLinks]") {
  const equilibrium::Graph graph{5, "", {{1, 2, 3, 4}, {0, 2}, {0, 1, 3}, {0, 2}, {0}}};
  REQUIRE(equilibrium::NumberOfUnmatchingLinks({2, 1, 2, 0, 2}, graph) == 4*2);
}

TEST_CASE("directed mixed population links", "[NumberOfUnmatchingLinks]") {
  const equilibrium::Graph graph{3, "", {{1}, {0, 2}, {}}};
  REQUIRE(equilibrium::NumberOfUnmatchingLinks({2, 1, 2}, graph) == 3);
}

TEST_CASE("N=1 links", "[NumberOfUnmatchingLinks]") {
  const equilibrium::Graph graph{1, "", {{}}};
  REQUIRE(equilibrium::NumberOfUnmatchingLinks({2}, graph) == 0);
}

TEST_CASE("star graph", "[StarGraph]") {
  const auto star = equilibrium::StarGraph(4);
  REQUIRE(star.size() == 4);
  REQUIRE(star.name() == "star");

  const std::vector<std::vector<int>> expected = {{1, 2, 3}, {0}, {0}, {0}};
  REQUIRE(star.out_edges() == expected);
}

TEST_CASE("cycle graph", "[CycleGraph]") {
  const auto cycle = equilibrium::CycleGraph(4);
  REQUIRE(cycle.size() == 4);
  REQUIRE(cycle.name() == "cycle");

  const std::vector<std::vector<int>> expected = {{1, 3}, {0, 2}, {1, 3}, {2, 0}};
  REQUIRE(cycle.out_edges() == expected);
}

TEST_CASE("complete graph", "[CompleteGraph]") {
  const auto complete = equilibrium::CompleteGraph(4);
  REQUIRE(complete.size() == 4);
  REQUIRE(complete.name() == "complete");

  const std::vector<std::vector<int>> expected = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};
  REQUIRE(complete.out_edges() == expected);
}

TEST_CASE("double star N=1", "[DoubleStarGraph]") {
  const auto double_star = equilibrium::DoubleStarGraph(1);
  REQUIRE(double_star.size() == 1);
  REQUIRE(double_star.name() == "double star");

  std::vector<std::vector<int>> expected;
  expected.resize(1);
  REQUIRE(double_star.out_edges() == expected);
}

TEST_CASE("double star N=2", "[DoubleStarGraph]") {
  const auto double_star = equilibrium::DoubleStarGraph(2);
  REQUIRE(double_star.size() == 2);
  REQUIRE(double_star.name() == "double star");

  std::vector<std::vector<int>> expected = {{1}, {0}};
  REQUIRE(double_star.out_edges() == expected);
}

TEST_CASE("double star N=3", "[DoubleStarGraph]") {
  const auto double_star = equilibrium::DoubleStarGraph(3);
  REQUIRE(double_star.size() == 3);
  REQUIRE(double_star.name() == "double star");

  std::vector<std::vector<int>> expected = {{1, 2}, {0}, {0}};
  REQUIRE(double_star.out_edges() == expected);
}

TEST_CASE("double star N=4", "[DoubleStarGraph]") {
  const auto double_star = equilibrium::DoubleStarGraph(4);
  REQUIRE(double_star.size() == 4);
  REQUIRE(double_star.name() == "double star");

  std::vector<std::vector<int>> expected = {{1, 2}, {0, 3}, {0}, {1}};
  REQUIRE(double_star.out_edges() == expected);
}

TEST_CASE("double star N=5", "[DoubleStarGraph]") {
  const auto double_star = equilibrium::DoubleStarGraph(5);
  REQUIRE(double_star.size() == 5);
  REQUIRE(double_star.name() == "double star");

  std::vector<std::vector<int>> expected = {{1, 2, 4}, {0, 3}, {0}, {1}, {0}};
  REQUIRE(double_star.out_edges() == expected);
}

TEST_CASE("undirected graph", "[IsBidirectional]") {
  const equilibrium::Graph graph{4, "", {{1}, {0, 2, 3}, {1, 3}, {1, 2}}};
  REQUIRE(equilibrium::IsBidirectional(graph));
}

TEST_CASE("directed graph", "[IsBidirectional]") {
  const equilibrium::Graph graph{4, "", {{1}, {2, 3}, {1, 3}, {1, 2}}};
  REQUIRE(!equilibrium::IsBidirectional(graph));
}

TEST_CASE("directed graph has correct in edges", "[ComputeInEdges]") {
  const equilibrium::Graph graph{4, "", {{1}, {2, 3}, {1, 3}, {1, 2}}};
  const std::vector<std::vector<int>> expected = {{}, {0, 2, 3}, {1, 3}, {1, 2}};
  REQUIRE(graph.in_edges() == expected);
}

