#include <cassert>
#include <map>
#include <random>
#include <set>

#include <equilibrium/simulation.h>

namespace equilibrium {

Stats Simulate(const SimulationConfig& config) {
  assert(config.graph.adjacency_list.size() == config.graph.N);

  // Initialize distributions.
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<> birther_dist(0, config.graph.N-1);
  std::vector<std::uniform_int_distribution<>> dier_idx_dists(config.graph.N);
  for (int i = 0; i < config.graph.N; ++i) {
    dier_idx_dists[i] = std::uniform_int_distribution<>(0, config.graph.adjacency_list[i].size()-1);
  }
  std::uniform_real_distribution<> birth_mutation_dist(0.0, 1.0);
  std::uniform_real_distribution<> independent_mutation_dist(0.0, 1.0);
  std::uniform_int_distribution<> independent_mutation_location_dist(0, config.graph.N-1);

  int max_type = 0;
  std::vector<int> location_to_type(config.graph.N, 0);

  // Evolve!
  for (int step = 0; step < config.num_steps; ++step) {
    // bd step.
    const auto birther = birther_dist(rng);
    const auto dier_idx = dier_idx_dists[birther](rng);
    const auto dier = config.graph.adjacency_list[birther][dier_idx];
    location_to_type[dier] = location_to_type[birther];

    // Possibly mutate birth.
    if (birth_mutation_dist(rng) < config.birth_mutation_rate) {
      location_to_type[dier] = ++max_type;
    }

    // Possibly independent mutation.
    if (independent_mutation_dist(rng) < config.independent_mutation_rate) {
      const auto mutated_location = independent_mutation_location_dist(rng);
      location_to_type[mutated_location] = ++max_type;
    }
  }

  // Record stats.
  Stats stats;
  stats.number_of_types = NumberOfTypes(location_to_type);
  stats.number_of_unmatching_pairs = NumberOfUnmatchingPairs(location_to_type);
  stats.number_of_unmatching_links = NumberOfUnmatchingLinks(location_to_type, config.graph);
  return stats;
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
    const auto stats = equilibrium::Simulate(config);

#pragma omp critical
    {
      for (const auto& measure : DIVERSITY_MEASURES) {
        ++(*diversity_counts)[measure][GetMeasureResult(stats, measure)];
      }
    }
  }
}

int NumberOfTypes(const std::vector<int>& location_to_type) {
  std::set<int> unique_elements(location_to_type.begin(), location_to_type.end());
  return unique_elements.size();
}

int NumberOfUnmatchingPairs(const std::vector<int>& location_to_type) {
  // Note: this assumes the graph is undirected.
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
  // Note: this assumes the graph is undirected.
  int num = 0;
  for (int i = 0; i < graph.N; ++i) {
    for (const auto& j : graph.adjacency_list[i]) {
      if ((i < j) && (location_to_type[i] != location_to_type[j])) {
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

}  // namespace equilibrium