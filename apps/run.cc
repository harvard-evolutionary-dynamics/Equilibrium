#include <iostream>
#include <map>

#include <omp.h>
#include <equilibrium/simulation.h>
#include <prettyprint.hpp>

equilibrium::Graph CompleteGraph(int N) {
  equilibrium::Graph graph;
  graph.N = N;
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

int main(int argc, char** argv) {
  equilibrium::SimulationConfig config;
  config.birth_mutation_rate = 1e-4;
  config.steps = 1e5;
  config.graph = CompleteGraph(100);

  std::map<int, int> diversity_counts;
#pragma omp parallel for num_threads(32)
  for (int trial = 0; trial < 1e3; ++trial) {
    const auto stats = equilibrium::Simulate(config);

#pragma omp critical
    {
      if (diversity_counts.find(stats.number_of_types) ==
          diversity_counts.end()) {
        diversity_counts[stats.number_of_types] = 0;
      }
      ++diversity_counts[stats.number_of_types];
    }
  }
  std::cout << "Diversity counts: " << diversity_counts << std::endl;
}