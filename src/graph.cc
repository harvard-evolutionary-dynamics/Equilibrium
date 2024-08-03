#include <set>
#include <string>
#include <utility>

#include <equilibrium/graph.h>

namespace equilibrium {

Graph::Graph(const int N, const std::string& name, const std::vector<std::vector<int>>& out_edges) : size_(N), name_(name), out_edges_(out_edges) {
 ComputeInEdges();
}

Graph CompleteGraph(int N) {
  std::vector<std::vector<int>> out_edges;
  out_edges.resize(N);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i != j) {
        out_edges[i].emplace_back(j);
      }
    }
  }

  return {N, "complete", out_edges};
}

Graph StarGraph(int N) {
  std::vector<std::vector<int>> out_edges;
  out_edges.resize(N);

  for (int j = 1; j < N; ++j) {
    out_edges[0].emplace_back(j);
    out_edges[j].emplace_back(0);
  }

  return {N, "star", out_edges};
}

Graph CycleGraph(int N) {
  std::vector<std::vector<int>> out_edges;
  out_edges.resize(N);

  for (int i = 0; i < N; ++i) {
    const auto next = (i+1) % N;
    out_edges[i].emplace_back(next);
    out_edges[next].emplace_back(i);
  }

  return {N, "cycle", out_edges};
}

Graph DoubleStarGraph(int N) {
  std::vector<std::vector<int>> out_edges;

  const auto left_star = StarGraph(N/2 + N%2);
  const auto right_star = StarGraph(N/2);

  out_edges.resize(N);

  if (N > 1) {
    out_edges[0].emplace_back(1);
    out_edges[1].emplace_back(0);
  }

  for (int j = 1; j < N/2 + N%2; ++j) {
    // Left.
    out_edges[0].emplace_back(2*j);
    out_edges[2*j].emplace_back(0);

    // Right.
    if (2*j+1 < N) {
      out_edges[1].emplace_back(2 * j + 1);
      out_edges[2 * j + 1].emplace_back(1);
    }
  }

  return {N, "double star", out_edges};
}

Graph DirectedLineGraph(int N) {
  std::vector<std::vector<int>> out_edges;
  out_edges.resize(N);
  for (int i = 0; i < N-1; ++i) {
    out_edges[i].emplace_back(i+1);
  }

  return {N, "directed line", out_edges};
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
  if (graph_name == "directed-line") {
    *graph = DirectedLineGraph(N);
    return true;
  }
  return false;
}

bool IsBidirectional(const Graph& graph) {
  std::set<std::pair<int, int>> edges;
  for (int i = 0; i < graph.size(); ++i) {
    for (const auto& j: graph.out_edges()[i]) {
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

void Graph::ComputeInEdges() {
  // Assumes graph.out_edges is already specified.
  in_edges_.clear();
  in_edges_.resize(size_);

  for (int i = 0; i < size_; ++i) {
    for (int j : out_edges_[i]) {
      in_edges_[j].emplace_back(i);
    }
  }
}

}  // namespace equilibrium