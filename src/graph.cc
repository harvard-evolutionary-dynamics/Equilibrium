#include <map>
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

Graph LineGraph(int N) {
  std::vector<std::vector<int>> out_edges;
  out_edges.resize(N);
  for (int i = 0; i < N-1; ++i) {
    out_edges[i].emplace_back(i+1);
    out_edges[i+1].emplace_back(i);
  }

  return {N, "line", out_edges};
}

Graph ContractingPathGraph(int N) {
  std::vector<std::vector<int>> out_edges;
  out_edges.resize(N);

  const int midpoint = N / 2;

  // Left spine.
  for (int i = 0; i < midpoint; ++i) {
    out_edges[i+1].emplace_back(i);
  }

  // Left Blade.
  for (int i = 0; i < midpoint; ++i) {
    for (int j = i+1; j <= midpoint; ++j) {
      out_edges[i].emplace_back(j);
    }
  }

  // Right Blade.
  for (int i = N-1; i > midpoint; --i) {
    for (int j = midpoint; j < i; ++j) {
      out_edges[i].emplace_back(j);
    }
  }

  // Right spine.
  for (int i = midpoint; i < N-1; ++i) {
    out_edges[i].emplace_back(i+1);
  }

  return {N, "contracting path", out_edges};
}

Graph BarbellGraph(int N) {
  const int N_clique = N / 3;
  const int N_path = N - 2 * N_clique;
  const Graph left_clique = CompleteGraph(N_clique);
  const Graph path = LineGraph(N_path);
  const Graph right_clique = CompleteGraph(N_clique);

  std::vector<std::vector<int>> out_edges;
  out_edges.resize(N);

  // Left clique.
  for (int u = 0; u < N_clique; ++u) {
    for (const auto& v : left_clique.out_edges()[u]) {
      out_edges[u].emplace_back(v);
    }
  }

  out_edges[N_clique-1].emplace_back(N_clique);
  out_edges[N_clique].emplace_back(N_clique-1);

  for (int u = 0; u < N_path; ++u) {
    for (const auto& v : path.out_edges()[u]) {
      out_edges[u+N_clique].emplace_back(v+N_clique);
    }
  }

  out_edges[N_clique+N_path-1].emplace_back(N_clique+N_path);
  out_edges[N_clique+N_path].emplace_back(N_clique+N_path-1);

  // Right clique.
  for (int u = 0; u < N_clique; ++u) {
    for (const auto& v : right_clique.out_edges()[u]) {
      out_edges[u+N_clique+N_path].emplace_back(v+N_clique+N_path);
    }
  }

  return {N, "barbell", out_edges};
}

const std::map<std::string, std::function<Graph(int)>> kGraphNameToGenerator {
    {"complete", CompleteGraph},
    {"star", StarGraph},
    {"cycle", CycleGraph},
    {"double-star", DoubleStarGraph},
    {"directed-line", DirectedLineGraph},
    {"line", LineGraph},
    {"contracting-path", ContractingPathGraph},
    {"barbell", BarbellGraph},
};

bool GetGraph(const std::string& graph_name, int N, Graph* graph) {
  if (kGraphNameToGenerator.find(graph_name) == kGraphNameToGenerator.end()) {
    return false;
  }
  const auto& graph_generator_fn = kGraphNameToGenerator.at(graph_name);
  *graph = graph_generator_fn(N);
  return true;
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