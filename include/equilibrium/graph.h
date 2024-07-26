#ifndef EQUILIBRIUM_GRAPH_H_
#define EQUILIBRIUM_GRAPH_H_

#include <string>
#include <vector>

namespace equilibrium {

struct Graph {
  int N;
  std::string name;
  std::vector<std::vector<int>> adjacency_list;
};

Graph CompleteGraph(int N);

/// Center node is 0; rest of nodes are 1, ..., N-1.
Graph StarGraph(int N);

/// Center node on left is 0; center node right is 1.
/// Even vertices are on left, odd vertices are on right.
Graph DoubleStarGraph(int N);

Graph CycleGraph(int N);

bool GetGraph(const std::string& graph_name, int N, Graph* graph);

} // namespace equilibrium

#endif // EQUILIBRIUM_GRAPH_H_
