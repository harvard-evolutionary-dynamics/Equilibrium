#ifndef EQUILIBRIUM_SIMULATION_H_
#define EQUILIBRIUM_SIMULATION_H_

#include <map>
#include <random>
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

enum class Dynamic {
  BIRTH_DEATH,
  DEATH_BIRTH,
};

bool FromString(const std::string&, Dynamic*);
bool ToString(const Dynamic&, std::string*);

struct Step {
  int birther;
  int dier;
};

// Only used for internal logic.
struct StepConfig {
  StepConfig(
    const Dynamic& dynamic,
    const Graph& graph,
    std::uniform_int_distribution<>& first_step_dist,
    std::vector<std::uniform_int_distribution<>>& second_step_idx_dists,
    std::mt19937& rng
  );

  const Dynamic& dynamic;
  const Graph& graph;
  std::uniform_int_distribution<>& first_step_dist;
  std::vector<std::uniform_int_distribution<>>& second_step_idx_dists;
  std::mt19937& rng;
};

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
  Dynamic dynamic;
  Graph graph;
  bool capture_history;
  bool compute_stats;
};

struct SimulationHistory {
  std::vector<std::vector<int>> location_to_types;
};

void BirthDeathStep(const StepConfig&, Step*);
void DeathBirthStep(const StepConfig&, Step*);
bool MakeStep(const StepConfig&, Step*);

void Simulate(const SimulationConfig&, Stats*, SimulationHistory*);

void ComputeDiversityCounts(const equilibrium::SimulationConfig&, DiversityCounts*);

int NumberOfTypes(const std::vector<int>&);

int NumberOfUnmatchingPairs(const std::vector<int>&);

int NumberOfUnmatchingLinks(const std::vector<int>&, const Graph&);

bool ToString(const DiversityMeasure& measure, std::string* output);

int GetMeasureResult(const Stats& stats, const DiversityMeasure& measure);


} // namespace equilibrium

#endif // EQUILIBRIUM_SIMULATION_H_
