#ifndef EQUILIBRIUM_WRITER_H_
#define EQUILIBRIUM_WRITER_H_

#include <chrono>
#include <ostream>
#include <string>

#include "simulation.h"

namespace equilibrium {

struct MetaData {
  std::chrono::time_point<std::chrono::system_clock> start_time;
  std::chrono::time_point<std::chrono::system_clock> end_time;
  std::string tag;
};

std::string GetOutputFileName(const std::string& prefix, const std::chrono::time_point<std::chrono::system_clock>& time);
void WriteDiversityCountsToStream(const DiversityCounts&, const SimulationConfig&, const MetaData&, std::ostream* os);
void WriteSimulationHistoryToStream(const SimulationHistory&, const SimulationConfig&, const MetaData&, std::ostream* os);

} // namespace equilibrium

#endif // EQUILIBRIUM_WRITER_H_
