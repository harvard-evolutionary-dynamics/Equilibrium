#ifndef EQUILIBRIUM_SIMULATION_H_
#define EQUILIBRIUM_SIMULATION_H_

#include "graph.h"

namespace equilibrium {

struct Stats {
  int number_of_types;
};

struct SimulationConfig {
  double birth_mutation_rate;
  int num_steps;
  int num_simulations;
  Graph graph;
};

Stats Simulate(const SimulationConfig&);

void ComputeDiversityCounts(const equilibrium::SimulationConfig&, std::map<int, int>*);

int NumberOfTypes(const std::vector<int>);

} // namespace equilibrium

#endif // EQUILIBRIUM_SIMULATION_H_
