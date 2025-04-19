#pragma once

#include <cstdint>
#include <fstream>
#include <optional>
#include <string>

struct TraceEntry {
  uint64_t branch_pc;
  uint64_t target_pc;
  bool taken;
};

class TraceReader {
public:
  explicit TraceReader(const std::string &file_path);
  ~TraceReader();

  // Get the next trace entry, if available
  std::optional<TraceEntry> next();

private:
  std::ifstream file_;
  uint32_t line_number_ = 0;
};
