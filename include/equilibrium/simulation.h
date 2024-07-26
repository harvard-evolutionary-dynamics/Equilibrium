#ifndef EQUILIBRIUM_SIMULATION_H_
#define EQUILIBRIUM_SIMULATION_H_

#include <map>
#include <vector>

#include "graph.h"


namespace equilibrium {

using DiversityCounts = std::map<int, int>;

struct Stats {
  int number_of_types;
};

struct SimulationConfig {
  double birth_mutation_rate;
  double independent_mutation_rate;
  int num_steps;
  int num_simulations;
  Graph graph;
};

Stats Simulate(const SimulationConfig&);

void ComputeDiversityCounts(const equilibrium::SimulationConfig&, DiversityCounts*);

int NumberOfTypes(const std::vector<int>);

} // namespace equilibrium

#endif // EQUILIBRIUM_SIMULATION_H_
