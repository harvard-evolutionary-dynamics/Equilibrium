#include <string>

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

bool GetGraph(const std::string& graph_name, int N, Graph* graph) {
  if (graph_name == "complete") {
    *graph = CompleteGraph(N);
    return true;
  }
  return false;
}

}  // namespace equilibrium