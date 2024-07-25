#ifndef EQUILIBRIUM_SIMULATION_H_
#define EQUILIBRIUM_SIMULATION_H_

#include <vector>

namespace equilibrium {

struct Graph {
  int N;
  std::vector<std::vector<int>> adjacency_list;
};

struct Stats {
  int number_of_types;
};

struct SimulationConfig {
  float birth_mutation_rate;
  int steps;
  Graph graph;
};

Stats Simulate(const SimulationConfig&);

int NumberOfTypes(const std::vector<int>);

} // namespace equilibrium

#endif // EQUILIBRIUM_SIMULATION_H_
