#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include <equilibrium/graph.h>
#include <equilibrium/simulation.h>
#include <equilibrium/writer.h>
#include <gflags/gflags.h>
#include <omp.h>
#include <prettyprint.hpp>

DEFINE_int32(N, 0, "N");
DEFINE_int32(num_steps, 0, "num-steps");
DEFINE_int32(num_simulations, 0, "num-simulations");
DEFINE_double(birth_mutation_rate, 0, "birth-mutation-rate");
DEFINE_double(independent_mutation_rate, 0, "independent-mutation-rate");
DEFINE_string(graph_name, "complete", "graph-name");


int main(int argc, char** argv) {

  gflags::SetUsageMessage(
      "Simulate birth-death process with multiple mutations");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  equilibrium::SimulationConfig config;
  config.birth_mutation_rate = FLAGS_birth_mutation_rate;
  config.independent_mutation_rate = FLAGS_independent_mutation_rate;
  config.num_steps = FLAGS_num_steps;
  config.num_simulations = FLAGS_num_simulations;

  if (!GetGraph(FLAGS_graph_name, FLAGS_N, &config.graph)) {
    throw std::invalid_argument("No graph named '" + FLAGS_graph_name + "'");
  }


  equilibrium::MetaData metadata;
  metadata.start_time = std::chrono::system_clock::now();

  equilibrium::DiversityCounts diversity_counts;
  ComputeDiversityCounts(config, &diversity_counts);
  std::cout << "Diversity counts: " << diversity_counts << std::endl;

  metadata.end_time = std::chrono::system_clock::now();

  const std::string output_file_name = equilibrium::GetOutputFileName(metadata.start_time);
  std::ofstream ofs{"data/" + output_file_name + ".json"};
  equilibrium::WriteDiversityCountsToStream(diversity_counts, config, metadata, &ofs);
}