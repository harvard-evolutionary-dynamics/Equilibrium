#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <cmath>

#include <equilibrium/graph.h>
#include <equilibrium/simulation.h>
#include <equilibrium/writer.h>
#include <gflags/gflags.h>
#include <omp.h>
#include <prettyprint.hpp>

DEFINE_int32(N, 0, "N");
DEFINE_int32(num_simulations, 1, "num-simulations");
DEFINE_string(graph_name, "complete", "graph-name");
DEFINE_string(dynamic, "birth-death", "dynamic");
DEFINE_double(exp_sample_rate, 1., "exp-sample-rate");
DEFINE_string(tag, "", "tag");

bool should_sample(int s, double exp_sample_rate) {
  return s == std::floor(std::pow(exp_sample_rate, std::ceil(std::log(s)/std::log(exp_sample_rate))));
}

void GetConfigs(const std::vector<int>& ns, std::map<int, equilibrium::SimulationConfig>* configs) {
  equilibrium::SimulationConfig config;
  config.birth_mutation_rate = 0;
  config.independent_mutation_rate = 0;
  config.num_steps = 0;
  config.num_simulations = FLAGS_num_simulations;
  config.history_sample_rate = -1;
  config.compute_stats = true;
  config.capture_history = false;
  config.start_with_max_diversity = true;
  config.run_until_homogeneous = true;

  if (config.run_until_homogeneous && config.num_steps) {
    throw std::invalid_argument(
        "Cannot specify 'run-until-homogeneous' and 'num-steps'");
  }

  if (!FromString(FLAGS_dynamic, &config.dynamic)) {
    throw std::invalid_argument("No dynamic named '" + FLAGS_dynamic + "'");
  }

  for (int n : ns) {
    equilibrium::SimulationConfig config_ = config;
    if (!GetGraph(FLAGS_graph_name, n, &config_.graph)) {
      throw std::invalid_argument("No graph named '" + FLAGS_graph_name +
                                  "'");
    }
    (*configs)[n] = config_;
  }
}

int main(int argc, char** argv) {
  gflags::SetUsageMessage(
      "Simulate birth-death process with multiple mutations: capture history");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::vector<int> ns;
  for (int n = 2; n <= FLAGS_N; ++n) {
    if (should_sample(n, FLAGS_exp_sample_rate)) {
      ns.push_back(n);
    }
  }

  std::map<int, equilibrium::SimulationConfig> configs;
  GetConfigs(ns, &configs);


  equilibrium::MetaData metadata;
  metadata.tag = FLAGS_tag;
  metadata.start_time = std::chrono::system_clock::now();

  equilibrium::Trends absorption_times;
#pragma omp parallel for collapse(2)
  for (int trial = 0; trial < FLAGS_num_simulations; ++trial) {
    for (const int n : ns) {
      equilibrium::Stats stats;
      equilibrium::Simulate(configs[n], &stats, nullptr);
#pragma omp critical
      { absorption_times[n].emplace_back(stats.number_of_steps); }
    }
  }

  std::cout << "done" << std::endl;
  metadata.end_time = std::chrono::system_clock::now();

  const std::string output_file_name = equilibrium::GetOutputFileName("trends-", metadata.start_time);
  std::ofstream ofs{"data/" + output_file_name + ".json"};
  equilibrium::WriteTrendsToStream(
   FLAGS_graph_name,
   FLAGS_N,
   FLAGS_exp_sample_rate,
   absorption_times,
   configs[ns[0]],
   metadata,
   &ofs
  );
}