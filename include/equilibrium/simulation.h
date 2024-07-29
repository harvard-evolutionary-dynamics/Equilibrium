#ifndef EQUILIBRIUM_SIMULATION_H_
#define EQUILIBRIUM_SIMULATION_H_

#include <map>
#include <vector>

#include "graph.h"


namespace equilibrium {

enum class DiversityMeasure {
  NUMBER_OF_TYPES,
  NUMBER_OF_UNMATCHING_PAIRS,
  NUMBER_OF_UNMATCHING_LINKS,
};

const std::vector<DiversityMeasure> DIVERSITY_MEASURES = {
  DiversityMeasure::NUMBER_OF_TYPES,
  DiversityMeasure::NUMBER_OF_UNMATCHING_PAIRS,
  DiversityMeasure::NUMBER_OF_UNMATCHING_LINKS
};

using DiversityCounts = std::map<DiversityMeasure, std::map<int, int>>;

struct Stats {
  int number_of_types;
  int number_of_unmatching_pairs;
  int number_of_unmatching_links;
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

int NumberOfTypes(const std::vector<int>&);

int NumberOfUnmatchingPairs(const std::vector<int>&);

int NumberOfUnmatchingLinks(const std::vector<int>&, const Graph&);

bool ToString(const DiversityMeasure& measure, std::string* output);

int GetMeasureResult(const Stats& stats, const DiversityMeasure& measure);

} // namespace equilibrium

#endif // EQUILIBRIUM_SIMULATION_H_
