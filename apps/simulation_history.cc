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
DEFINE_double(birth_mutation_rate, 0, "birth-mutation-rate");
DEFINE_double(independent_mutation_rate, 0, "independent-mutation-rate");
DEFINE_string(graph_name, "complete", "graph-name");
DEFINE_string(dynamic, "birth-death", "dynamic");
DEFINE_string(tag, "", "tag");
DEFINE_int32(sample_rate, 1, "sample-rate");


int main(int argc, char** argv) {

  gflags::SetUsageMessage(
      "Simulate birth-death process with multiple mutations: capture history");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  equilibrium::SimulationConfig config;
  config.birth_mutation_rate = FLAGS_birth_mutation_rate;
  config.independent_mutation_rate = FLAGS_independent_mutation_rate;
  config.num_steps = FLAGS_num_steps;
  config.history_sample_rate = FLAGS_sample_rate;
  config.num_simulations = 1;
  config.compute_stats = false;
  config.capture_history = true;

  if (!GetGraph(FLAGS_graph_name, FLAGS_N, &config.graph)) {
    throw std::invalid_argument("No graph named '" + FLAGS_graph_name + "'");
  }

  if (!FromString(FLAGS_dynamic, &config.dynamic)) {
    throw std::invalid_argument("No dynamic named '" + FLAGS_dynamic + "'");
  }


  equilibrium::MetaData metadata;
  metadata.tag = FLAGS_tag;
  metadata.start_time = std::chrono::system_clock::now();


  equilibrium::SimulationHistory history;
  equilibrium::Simulate(config, nullptr, &history);
  std::cout << "done" << std::endl;

  metadata.end_time = std::chrono::system_clock::now();

  const std::string output_file_name = equilibrium::GetOutputFileName("simulation-history-", metadata.start_time);
  std::ofstream ofs{"data/" + output_file_name + ".json"};

  equilibrium::WriteSimulationHistoryToStream(history, config, metadata, &ofs);
}