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

DEFINE_int32(N, 1, "N");
DEFINE_int32(num_steps, 0, "num-steps");
DEFINE_int32(num_simulations, 0, "num-simulations");
DEFINE_double(birth_mutation_rate, 0, "birth-mutation-rate");
DEFINE_double(independent_mutation_rate, 0, "independent-mutation-rate");
DEFINE_string(graph_name, "complete", "graph-name");
DEFINE_string(dynamic, "birth-death", "dynamic");
DEFINE_string(tag, "", "tag");


int main(int argc, char** argv) {
  gflags::SetUsageMessage(
      "Simulate birth-death process with multiple mutations: capture final diversity");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  equilibrium::SimulationConfig config;
  config.birth_mutation_rate = FLAGS_birth_mutation_rate;
  config.independent_mutation_rate = FLAGS_independent_mutation_rate;
  config.num_steps = FLAGS_num_steps;
  config.num_simulations = FLAGS_num_simulations;
  config.compute_stats = true;
  config.capture_history = false;

  if (!GetGraph(FLAGS_graph_name, FLAGS_N, &config.graph)) {
    throw std::invalid_argument("No graph named '" + FLAGS_graph_name + "'");
  }

  if (!FromString(FLAGS_dynamic, &config.dynamic)) {
    throw std::invalid_argument("No dynamic named '" + FLAGS_dynamic + "'");
  }


  equilibrium::MetaData metadata;
  metadata.tag = FLAGS_tag;
  metadata.start_time = std::chrono::system_clock::now();

  equilibrium::DiversityCounts diversity_counts;
  ComputeDiversityCounts(config, &diversity_counts);
  std::cout << "done" << std::endl;

  metadata.end_time = std::chrono::system_clock::now();

  const std::string output_file_name = equilibrium::GetOutputFileName("diversity-counts-", metadata.start_time);
  std::ofstream ofs{"data/" + output_file_name + ".json"};
  equilibrium::WriteDiversityCountsToStream(diversity_counts, config, metadata, &ofs);
}