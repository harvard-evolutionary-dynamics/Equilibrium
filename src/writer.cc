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

std::string GetOutputFileName(const std::chrono::time_point<std::chrono::system_clock>& time) {
  const auto milliseconds = GetMilliSeconds(time);

  std::stringstream ss;
  ss << "equilibrium-run-";
  ss << milliseconds;

  return ss.str();
}

void WriteDiversityCountsToStream(
        const DiversityCounts& diversity_counts,
        const SimulationConfig& config,
        const MetaData& metadata,
        std::ostream* os
) {
  nlohmann::json j;
  auto& config_json = j["config"];
  config_json["graph_name"] = config.graph.name;
  config_json["birth_mutation_rate"] = config.birth_mutation_rate;
  config_json["N"] = config.graph.N;
  config_json["num_steps"] = config.num_steps;
  config_json["num_simulations"] = config.num_simulations;

  auto& metadata_json = j["metadata"];
  metadata_json["start_time_s"] = GetSeconds(metadata.start_time);
  metadata_json["end_time_s"] = GetSeconds(metadata.end_time);

  auto& results_json = j["results"];
  auto& dc_json  = results_json["diversity_counts"];
  for (const auto& element : diversity_counts) {
    nlohmann::json single_diversity_count_json;
    single_diversity_count_json["diversity"] = element.first;
    single_diversity_count_json["count"] = element.second;

    dc_json.emplace_back(single_diversity_count_json);
  }

  (*os) << j.dump(2);
}

} // namespace equilibrium