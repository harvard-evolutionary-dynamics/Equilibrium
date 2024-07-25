#include <cassert>
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
  std::vector<std::uniform_int_distribution<>> dier_dists(config.graph.N);
  for (int i = 0; i < config.graph.N; ++i) {
    dier_dists[i] = std::uniform_int_distribution<>(0, config.graph.adjacency_list[i].size()-1);
  }
  std::uniform_real_distribution<> birth_mutation_dist(0.0, 1.0);

  int max_type = 0;
  std::vector<int> location_to_type(config.graph.N, 0);

  // Evolve!
  for (int step = 0; step < config.steps; ++step) {
    // bd step.
    const auto birther = birther_dist(rng);
    const auto dier = dier_dists[birther](rng);
    location_to_type[dier] = location_to_type[birther];

    // Possibly mutate birth.
    if (birth_mutation_dist(rng) < config.birth_mutation_rate) {
      location_to_type[dier] = ++max_type;
    }
  }

  // Record stats.
  Stats stats;
  stats.number_of_types = NumberOfTypes(location_to_type);
  return stats;
}


int NumberOfTypes(const std::vector<int> location_to_type) {
  std::set<int> unique_elements(location_to_type.begin(), location_to_type.end());
  return unique_elements.size();
}

}  // namespace equilibrium