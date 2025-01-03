#include <cassert>
#include <map>
#include <random>
#include <set>

#include <equilibrium/simulation.h>

namespace equilibrium {

StepConfig::StepConfig(
    const Dynamic& dynamic,
    const Graph& graph,
    std::uniform_int_distribution<>& first_step_dist,
    std::vector<std::uniform_int_distribution<>>& second_step_idx_dists,
    std::mt19937& rng
) : dynamic(dynamic),
    graph(graph),
    first_step_dist(first_step_dist),
    second_step_idx_dists(second_step_idx_dists), rng(rng) {}

bool BirthDeathStep(const StepConfig& step_config, Step* step) {
  // bd step.
  step->birther = step_config.first_step_dist(step_config.rng);
  const auto dier_idx = step_config.second_step_idx_dists[step->birther](step_config.rng);
  if (step_config.graph.out_edges()[step->birther].empty()) return false;
  step->dier = step_config.graph.out_edges()[step->birther][dier_idx];
  return true;
}

bool DeathBirthStep(const StepConfig& step_config, Step* step) {
  // db step.
  step->dier = step_config.first_step_dist(step_config.rng);
  const auto birther_idx = step_config.second_step_idx_dists[step->dier](step_config.rng);
  if (step_config.graph.in_edges()[step->dier].empty()) return false;
  step->birther = step_config.graph.in_edges()[step->dier][birther_idx];
  return true;
}

bool MakeStep(const StepConfig& step_config, Step* step) {
  if (step_config.dynamic == Dynamic::BIRTH_DEATH) {
    return BirthDeathStep(step_config, step);
  }
  if (step_config.dynamic == Dynamic::DEATH_BIRTH) {
    return DeathBirthStep(step_config, step);
  }
  return false;
}

void Simulate(const SimulationConfig& config, Stats* stats, SimulationHistory* history) {
  assert(config.graph.size() > 0);
  assert(config.graph.out_edges().size() == config.graph.size());
  // assert(!config.capture_history || config.num_simulations == 1);
  assert(!config.capture_history || history != nullptr);
  assert(config.capture_history || history == nullptr);
  assert(!config.compute_stats || stats != nullptr);
  assert(config.compute_stats || stats == nullptr);

  // Initialize distributions.
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<> first_step_dist(0, config.graph.size()-1);
  std::vector<std::uniform_int_distribution<>> second_step_idx_dists(config.graph.size());

  // Assuming there are only two dynamics: bd and db.
  const auto& neighbors = config.dynamic == Dynamic::BIRTH_DEATH ? config.graph.out_edges() : config.graph.in_edges();
  for (int i = 0; i < config.graph.size(); ++i) {
    if (neighbors[i].size() > 0) {
      second_step_idx_dists[i] = std::uniform_int_distribution<>(0, neighbors[i].size()-1);
    }
  }
  std::uniform_real_distribution<> birth_mutation_dist(0.0, 1.0);
  std::uniform_real_distribution<> independent_mutation_dist(0.0, 1.0);
  std::uniform_int_distribution<> independent_mutation_location_dist(0, config.graph.size()-1);

  // Configure the step options.
  StepConfig step_config(config.dynamic, config.graph, first_step_dist, second_step_idx_dists, rng);
  Step step;

  int max_type = 0;
  std::vector<int> location_to_type(config.graph.size(), 0);
  if (config.start_with_max_diversity) {
    for (int i = 0; i < location_to_type.size(); ++i) {
      location_to_type[i] = i;
    }
    max_type = config.graph.size() - 1;
  }
  if (config.capture_history && history != nullptr) {
    history->location_to_types.reserve(config.num_steps / config.history_sample_rate + 1);
    history->location_to_types.emplace_back(location_to_type);

    history->ancestry.reserve(config.num_steps + 1);
    // Type 0 has no ancestry.
    history->ancestry.emplace_back(kNoAncestry);
  }

  // Evolve!
  int step_num;
  for (step_num = 1;
       config.run_until_homogeneous ? !IsHomogeneous(location_to_type) : (step_num <= config.num_steps);
       ++step_num) {
    if (MakeStep(step_config, &step)) {
      location_to_type[step.dier] = location_to_type[step.birther];

      // Possibly mutate birth.
      if (birth_mutation_dist(rng) < config.birth_mutation_rate) {
        location_to_type[step.dier] = ++max_type;

        if (config.capture_history && history != nullptr) {
          history->ancestry.emplace_back(location_to_type[step.birther]);
          assert(history->ancestry.size() == max_type+1);
        }
      }
    }

    // Possibly independent mutation.
    if (independent_mutation_dist(rng) < config.independent_mutation_rate) {
      const auto mutated_location = independent_mutation_location_dist(rng);
      location_to_type[mutated_location] = ++max_type;

      if (config.capture_history && history != nullptr) {
        history->ancestry.emplace_back(kNoAncestry);
        assert(history->ancestry.size() == max_type+1);
      }
    }

    // This is slow because of a copy, be careful!
    if (config.capture_history && history != nullptr && step_num % config.history_sample_rate == 0) {
      history->location_to_types.emplace_back(location_to_type);
    }
  }

  // Record stats.
  if (config.compute_stats) {
    stats->number_of_types = NumberOfTypes(location_to_type);
    stats->number_of_unmatching_pairs =
        NumberOfUnmatchingPairs(location_to_type);
    stats->number_of_unmatching_links =
        NumberOfUnmatchingLinks(location_to_type, config.graph);
    stats->number_of_steps = step_num-1;
  }
}

void ComputeSimulationHistories(
    const SimulationConfig& config,
    SimulationHistories* simulation_histories
) {
#pragma omp parallel for
  for (int trial = 0; trial < config.num_simulations; ++trial) {
    equilibrium::SimulationHistory history;
    equilibrium::Simulate(config, nullptr, &history);

#pragma omp critical
    {
      simulation_histories->emplace_back(history);
    }
  }
}

int GetMeasureResult(const Stats& stats, const DiversityMeasure& measure) {
  if (measure == DiversityMeasure::NUMBER_OF_TYPES) return stats.number_of_types;
  if (measure == DiversityMeasure::NUMBER_OF_UNMATCHING_PAIRS) return stats.number_of_unmatching_pairs;
  if (measure == DiversityMeasure::NUMBER_OF_UNMATCHING_LINKS) return stats.number_of_unmatching_links;
  return -1;
}

void ComputeDiversityCounts(
    const equilibrium::SimulationConfig& config,
    DiversityCounts* diversity_counts
) {
#pragma omp parallel for
  for (int trial = 0; trial < config.num_simulations; ++trial) {
    Stats stats;
    equilibrium::Simulate(config, &stats, nullptr);

#pragma omp critical
    {
      for (const auto& measure : DIVERSITY_MEASURES) {
        ++(*diversity_counts)[measure][GetMeasureResult(stats, measure)];
      }
    }
  }
}

void ComputeTrends(
    const equilibrium::SimulationConfig& config,
    std::map<int, int>* absorption_times
) {

}

bool IsHomogeneous(const std::vector<int>& location_to_type) {
  return NumberOfTypes(location_to_type) == 1;
}

int NumberOfTypes(const std::vector<int>& location_to_type) {
  std::set<int> unique_elements(location_to_type.begin(), location_to_type.end());
  return unique_elements.size();
}

int NumberOfUnmatchingPairs(const std::vector<int>& location_to_type) {
  // Note: this considers unordered pairs exactly once.
  int num = 0;
  for (int i = 0; i < location_to_type.size(); ++i) {
    for (int j = i+1; j < location_to_type.size(); ++j) {
      if (location_to_type[i] != location_to_type[j]) {
        ++num;
      }
    }
  }

  return num;
}

int NumberOfUnmatchingLinks(const std::vector<int>& location_to_type, const Graph& graph) {
  int num = 0;
  for (int i = 0; i < graph.size(); ++i) {
    for (const auto& j : graph.out_edges()[i]) {
      if (location_to_type[i] != location_to_type[j]) {
        ++num;
      }
    }
  }

  return num;
}

bool ToString(const DiversityMeasure& measure, std::string* output) {
  if (measure == DiversityMeasure::NUMBER_OF_TYPES) {
    *output = "number_of_types";
    return true;
  }
  if (measure == DiversityMeasure::NUMBER_OF_UNMATCHING_PAIRS) {
    *output = "number_of_unmatching_pairs";
    return true;
  }
  if (measure == DiversityMeasure::NUMBER_OF_UNMATCHING_LINKS) {
    *output = "number_of_unmatching_links";
    return true;
  }
  return false;
}

bool FromString(const std::string& dynamic_str, Dynamic* dynamic) {
  if (dynamic_str == "birth-death") {
    *dynamic = Dynamic::BIRTH_DEATH;
    return true;
  }
  if (dynamic_str == "death-birth") {
    *dynamic = Dynamic::DEATH_BIRTH;
    return true;
  }

  return false;
}

bool ToString(const Dynamic& dynamic, std::string* dynamic_str) {
  if (dynamic == Dynamic::BIRTH_DEATH) {
    *dynamic_str = "birth-death";
    return true;
  }
  if (dynamic == Dynamic::DEATH_BIRTH) {
    *dynamic_str = "death-birth";
    return true;
  }

  return false;
}

}  // namespace equilibrium