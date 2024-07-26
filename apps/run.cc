#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include <equilibrium/graph.h>
#include <equilibrium/simulation.h>
#include <gflags/gflags.h>
#include <omp.h>
#include <prettyprint.hpp>

DEFINE_int32(N, 0, "N");
DEFINE_int32(num_steps, 0, "num-steps");
DEFINE_int32(num_simulations, 0, "num-simulations");
DEFINE_double(birth_mutation_rate, 0, "birth-mutation-rate");
DEFINE_string(graph_name, "complete", "graph-name");


int main(int argc, char** argv) {
  gflags::SetUsageMessage(
      "Simulate birth-death process with multiple mutations");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  equilibrium::SimulationConfig config;
  config.birth_mutation_rate = FLAGS_birth_mutation_rate;
  config.num_steps = FLAGS_num_steps;
  config.num_simulations = FLAGS_num_simulations;

  if (!GetGraph(FLAGS_graph_name, FLAGS_N, &config.graph)) {
    throw std::invalid_argument("No graph named '" + FLAGS_graph_name + "'");
  }

  std::map<int, int> diversity_counts;
  ComputeDiversityCounts(config, &diversity_counts);
  std::cout << "Diversity counts: " << diversity_counts << std::endl;
}