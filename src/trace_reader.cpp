#include "../include/trace_reader.h"
#include <sstream>
#include <stdexcept>

TraceReader::TraceReader(const std::string &file_path) : file_(file_path) {
  if (!file_) {
    throw std::runtime_error("Failed to open trace file: " + file_path);
  }
}

TraceReader::~TraceReader() = default;

std::optional<TraceEntry> TraceReader::next() {
  std::string line;
  if (!std::getline(file_, line)) {
    return std::nullopt;
  }

  line_number_++;
  std::istringstream iss(line);
  std::string branch_str, target_str, outcome_str;

  if (!(iss >> branch_str >> target_str >> outcome_str)) {
    throw std::runtime_error("Invalid trace format at line " +
                             std::to_string(line_number_));
  }

  try {
    TraceEntry entry;
    entry.branch_pc = std::stoull(branch_str, nullptr, 16);
    entry.target_pc = std::stoull(target_str, nullptr, 16);
    entry.taken = (outcome_str == "1");
    return entry;
  } catch (...) {
    throw std::runtime_error("Invalid data at line " +
                             std::to_string(line_number_));
  }
}
