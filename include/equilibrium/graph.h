#ifndef EQUILIBRIUM_GRAPH_H_
#define EQUILIBRIUM_GRAPH_H_

#include <string>
#include <vector>

namespace equilibrium {

struct Graph {
  int N;
  std::vector<std::vector<int>> adjacency_list;
};

Graph CompleteGraph(int N);

bool GetGraph(const std::string& graph_name, int N, Graph* graph);

} // namespace equilibrium

#endif // EQUILIBRIUM_GRAPH_H_
