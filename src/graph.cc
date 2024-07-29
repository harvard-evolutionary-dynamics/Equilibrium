#include <set>
#include <string>
#include <utility>

#include <equilibrium/graph.h>

namespace equilibrium {

Graph CompleteGraph(int N) {
  Graph graph;
  graph.N = N;
  graph.name = "complete";
  graph.adjacency_list.resize(N);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i != j) {
        graph.adjacency_list[i].emplace_back(j);
      }
    }
  }
  return graph;
}

Graph StarGraph(int N) {
  Graph graph;
  graph.N = N;
  graph.name = "star";
  graph.adjacency_list.resize(N);

  for (int j = 1; j < N; ++j) {
    graph.adjacency_list[0].emplace_back(j);
    graph.adjacency_list[j].emplace_back(0);
  }

  return graph;
}

Graph CycleGraph(int N) {
  Graph graph;
  graph.N = N;
  graph.name = "cycle";
  graph.adjacency_list.resize(N);

  for (int i = 0; i < N; ++i) {
    const auto next = (i+1) % N;
    graph.adjacency_list[i].emplace_back(next);
    graph.adjacency_list[next].emplace_back(i);
  }

  return graph;
}

Graph DoubleStarGraph(int N) {
  Graph graph;
  graph.N = N;
  graph.name = "double star";

  const auto left_star = StarGraph(N/2 + N%2);
  const auto right_star = StarGraph(N/2);

  graph.adjacency_list.resize(N);

  if (N > 1) {
    graph.adjacency_list[0].emplace_back(1);
    graph.adjacency_list[1].emplace_back(0);
  }

  for (int j = 1; j < N/2 + N%2; ++j) {
    // Left.
    graph.adjacency_list[0].emplace_back(2*j);
    graph.adjacency_list[2*j].emplace_back(0);

    // Right.
    if (2*j+1 < N) {
      graph.adjacency_list[1].emplace_back(2 * j + 1);
      graph.adjacency_list[2 * j + 1].emplace_back(1);
    }
  }

  return graph;
}

bool GetGraph(const std::string& graph_name, int N, Graph* graph) {
  if (graph_name == "complete") {
    *graph = CompleteGraph(N);
    return true;
  }
  if (graph_name == "star") {
    *graph = StarGraph(N);
    return true;
  }
  if (graph_name == "cycle") {
    *graph = CycleGraph(N);
    return true;
  }
  if (graph_name == "double-star") {
    *graph = DoubleStarGraph(N);
    return true;
  }
  return false;
}

bool IsUndirected(const Graph& graph) {
  std::set<std::pair<int, int>> edges;
  for (int i = 0; i < graph.N; ++i) {
    for (const auto& j: graph.adjacency_list[i]) {
      edges.insert({i, j});
    }
  }

  for (const auto& edge : edges) {
    // We should be able to find the reverse pair.
    if (edges.find({edge.second, edge.first}) == edges.end()) {
      return false;
    }
  }

  return true;
}

}  // namespace equilibrium