#ifndef EQUILIBRIUM_GRAPH_H_
#define EQUILIBRIUM_GRAPH_H_

#include <string>
#include <vector>

namespace equilibrium {

class Graph {
 public:
  Graph() = default;
  Graph(const int size, const std::string& name, const std::vector<std::vector<int>>& out_edges);

  const int& size() const { return size_; }
  const std::string& name() const { return name_; }
  const std::vector<std::vector<int>>& out_edges() const { return out_edges_; }
  const std::vector<std::vector<int>>& in_edges() const { return in_edges_; }

 private:
  void ComputeInEdges();

  int size_;
  std::string name_;
  std::vector<std::vector<int>> out_edges_;
  std::vector<std::vector<int>> in_edges_;
};

Graph CompleteGraph(int N);

/// Center node is 0; rest of nodes are 1, ..., N-1.
Graph StarGraph(int N);

/// Center node on left is 0; center node right is 1.
/// Even vertices are on left, odd vertices are on right.
Graph DoubleStarGraph(int N);

Graph CycleGraph(int N);

/// Root is 0, leaf is N-1.
Graph DirectedLineGraph(int N);

bool GetGraph(const std::string& graph_name, int N, Graph* graph);

bool IsBidirectional(const Graph&);


} // namespace equilibrium

#endif // EQUILIBRIUM_GRAPH_H_
