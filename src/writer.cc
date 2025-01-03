#include <ostream>
#include <sstream>
#include <string>

#include <equilibrium/simulation.h>
#include <equilibrium/writer.h>

#include <nlohmann/json.hpp>

namespace equilibrium {

long long GetNanoSeconds(const std::chrono::time_point<std::chrono::system_clock>& time) {
  const auto duration = time.time_since_epoch();
  const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
  return nanoseconds;
}

long long GetMilliSeconds(const std::chrono::time_point<std::chrono::system_clock>& time) {
  const auto duration = time.time_since_epoch();
  const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  return milliseconds;
}

long long GetSeconds(const std::chrono::time_point<std::chrono::system_clock>& time) {
  const auto duration = time.time_since_epoch();
  const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
  return seconds;
}

std::string GetOutputFileName(
  const std::string& prefix,
  const std::chrono::time_point<std::chrono::system_clock>& time
) {
  const auto milliseconds = GetMilliSeconds(time);

  std::stringstream ss;
  ss << prefix;
  ss << milliseconds;

  return ss.str();
}

void WriteTrendsToStream(
    const std::string& graph_name,
    const int N,
    const double exp_sample_rate,
    const Trends& trends,
    const SimulationConfig& config,
    const MetaData& metadata,
    std::ostream* os
) {
  nlohmann::json j;
  auto& config_json = j["config"];
  config_json["graph_name"] = graph_name;
  config_json["N"] = N;
  config_json["num_simulations"] = config.num_simulations;
  config_json["exp_sample_rate"] = exp_sample_rate;

  std::string dynamic_str;
  if (!ToString(config.dynamic, &dynamic_str)) {
    dynamic_str = "unknown";
  }
  config_json["dynamic"] = dynamic_str;

  auto& metadata_json = j["metadata"];
  metadata_json["start_time_s"] = GetSeconds(metadata.start_time);
  metadata_json["end_time_s"] = GetSeconds(metadata.end_time);
  metadata_json["tag"] = metadata.tag;

  auto& results_json = j["results"];

  for (const auto& trend : trends) {
    nlohmann::json result_json;
    result_json["n"] = trend.first;
    result_json["times"] = trend.second;
    results_json.emplace_back(result_json);
  }
  (*os) << j;
}

void WriteSimulationHistoryToStream(
  const SimulationHistories& histories,
  const SimulationConfig& config,
  const MetaData& metadata,
  std::ostream* os
) {
  nlohmann::json j;
  auto& config_json = j["config"];
  config_json["graph_name"] = config.graph.name();
  config_json["birth_mutation_rate"] = config.birth_mutation_rate;
  config_json["independent_mutation_rate"] = config.independent_mutation_rate;
  config_json["N"] = config.graph.size();
  config_json["num_steps"] = config.num_steps;
  config_json["num_simulations"] = config.num_simulations;
  config_json["sample_rate"] = config.history_sample_rate;
  config_json["start_with_max_diversity"] = config.start_with_max_diversity;
  config_json["run_until_homogeneous"] = config.run_until_homogeneous;

  std::string dynamic_str;
  if (!ToString(config.dynamic, &dynamic_str)) {
    dynamic_str = "unknown";
  }
  config_json["dynamic"] = dynamic_str;

  auto& metadata_json = j["metadata"];
  metadata_json["start_time_s"] = GetSeconds(metadata.start_time);
  metadata_json["end_time_s"] = GetSeconds(metadata.end_time);
  metadata_json["tag"] = metadata.tag;

  auto& results_json = j["results"];

  for (const auto& history : histories) {
    nlohmann::json result_json;
    auto& simulation_history_json = result_json["simulation_history"];
    for (int step_num = 0; step_num < history.location_to_types.size();
         ++step_num) {
      nlohmann::json time_slice;
      time_slice["step_num"] = step_num * config.history_sample_rate;
      time_slice["location_to_type"] = history.location_to_types[step_num];
      simulation_history_json.emplace_back(time_slice);
    }

    auto& ancestry_json = result_json["ancestry"];
    for (int child_type = 0; child_type < history.ancestry.size();
         ++child_type) {
      nlohmann::json parent_child;
      parent_child["child"] = child_type;
      parent_child["parent"] = history.ancestry[child_type];
      ancestry_json.emplace_back(parent_child);
    }

    results_json.emplace_back(result_json);
  }
  (*os) << j;
}

void WriteDiversityCountsToStream(
  const DiversityCounts& diversity_counts,
  const SimulationConfig& config,
  const MetaData& metadata,
  std::ostream* os
) {
  nlohmann::json j;
  auto& config_json = j["config"];
  config_json["graph_name"] = config.graph.name();
  config_json["birth_mutation_rate"] = config.birth_mutation_rate;
  config_json["independent_mutation_rate"] = config.independent_mutation_rate;
  config_json["N"] = config.graph.size();
  config_json["num_steps"] = config.num_steps;
  config_json["num_simulations"] = config.num_simulations;
  config_json["start_with_max_diversity"] = config.start_with_max_diversity;
  config_json["run_until_homogeneous"] = config.run_until_homogeneous;

  std::string dynamic_str;
  if (!ToString(config.dynamic, &dynamic_str)) {
    dynamic_str = "unknown";
  }
  config_json["dynamic"] = dynamic_str;

  auto& metadata_json = j["metadata"];
  metadata_json["start_time_s"] = GetSeconds(metadata.start_time);
  metadata_json["end_time_s"] = GetSeconds(metadata.end_time);
  metadata_json["tag"] = metadata.tag;

  auto& results_json = j["results"];
  for (const auto& measure_counts : diversity_counts) {
    nlohmann::json diversity_count_json;

    std::string diversity_measure;
    if (!ToString(measure_counts.first, &diversity_measure)) {
      diversity_measure = "unknown";
    }
    diversity_count_json["diversity_measure"] = diversity_measure;

    auto& dc_json  = diversity_count_json["diversity_counts"];
    for (const auto& element : measure_counts.second) {
      nlohmann::json count_json;
      count_json["diversity"] = element.first;
      count_json["count"] = element.second;
      dc_json.emplace_back(count_json);
    }

    results_json.emplace_back(diversity_count_json);
  }

  (*os) << j.dump(2);
}

} // namespace equilibrium