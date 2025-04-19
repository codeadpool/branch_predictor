#include "../include/OneBitPredictor.h"
#include "../include/static_bp.h"
#include "../include/trace_reader.h"

#include <iostream>
#include <memory>
#include <string>

void print_help(const std::string &program_name) {
  std::cout
      << "Usage: " << program_name << " [OPTIONS] TRACE_FILE\n"
      << "Options:\n"
      << "  -p PREDICTOR  Select predictor: static_taken, static_not_taken, "
         "one_bit\n"
      << "                (default: one_bit)\n"
      << "  -s SIZE       Set predictor table size (default: "
      << OneBitPredictor::DEFAULT_TABLE_SIZE << ", rounded to power of 2)\n"
      << "  -b BITS       Set history bits for GShare (default: 8)\n"
      << "  -h            Show this help message\n"
      << "\n"
      << "TRACE_FILE format: Each line contains BRANCH_PC TARGET_PC OUTCOME\n"
      << "  (BRANCH_PC and TARGET_PC in hex, OUTCOME=1 for taken, 0 for not "
         "taken)\n"
      << "Example: ./branch_predictor -p gshare -s 2048 -b 10 "
         "traces/sample_trace.txt\n";
}

int main(int argc, char *argv[]) {
  std::string predictor_type = "one_bit";
  uint32_t table_size = OneBitPredictor::DEFAULT_TABLE_SIZE;
  uint32_t history_bits = 8;
  std::string trace_file;

  // Parse command-line arguments
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-h" || arg == "--help") {
      print_help(argv[0]);
      return 0;
    } else if (arg == "-p" && i + 1 < argc) {
      predictor_type = argv[++i];
    } else if (arg == "-s" && i + 1 < argc) {
      try {
        table_size = std::stoul(argv[++i]);
        if (table_size == 0) {
          std::cerr << "Error: Table size must be positive\n";
          return 1;
        }
      } catch (...) {
        std::cerr << "Error: Invalid table size '" << argv[i] << "'\n";
        return 1;
      }
    } else if (arg == "-b" && i + 1 < argc) {
      try {
        history_bits = std::stoul(argv[++i]);
        if (history_bits > 32) {
          std::cerr << "Error: History bits must be <= 32\n";
          return 1;
        }
      } catch (...) {
        std::cerr << "Error: Invalid history bits '" << argv[i] << "'\n";
        return 1;
      }
    } else if (arg[0] != '-') {
      trace_file = arg;
    } else {
      std::cerr << "Error: Unknown option '" << arg << "'\n";
      print_help(argv[0]);
      return 1;
    }
  }

  if (trace_file.empty()) {
    std::cerr << "Error: No trace file specified\n";
    print_help(argv[0]);
    return 1;
  }

  try {
    // Create predictor based on type
    std::unique_ptr<BranchPredictor> predictor;
    if (predictor_type == "static_taken") {
      predictor = std::make_unique<StaticBranchPredictor>(true);
    } else if (predictor_type == "static_not_taken") {
      predictor = std::make_unique<StaticBranchPredictor>(false);
    } else if (predictor_type == "one_bit") {
      predictor = std::make_unique<OneBitPredictor>(table_size);
    } else {
      std::cerr << "Error: Unknown predictor '" << predictor_type << "'\n";
      print_help(argv[0]);
      return 1;
    }

    // Print configuration
    std::cout << "=== Branch Predictor Simulation ===\n";
    std::cout << "Configuration:\n";
    std::cout << "  Predictor: " << predictor->get_name() << "\n";
    std::cout << "  Table Size: " << table_size
              << " (rounded to next power of 2)\n";
    if (predictor_type == "gshare") {
      std::cout << "  History Bits: " << history_bits << "\n";
    }
    std::cout << "  Trace File: " << trace_file << "\n\n";

    // Run simulation
    TraceReader reader(trace_file);
    std::cout << "Running simulation...\n";
    while (auto entry = reader.next()) {
      bool prediction = predictor->predict(entry->branch_pc);
      predictor->update(entry->branch_pc, entry->taken);
    }

    predictor->print_stats();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
