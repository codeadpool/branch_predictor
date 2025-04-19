#pragma once

#include "branch_predictor.h"
#include <cstdint>
#include <string>
#include <vector>

class OneBitPredictor : public BranchPredictor {
private:
  std::vector<bool> prediction_table_;
  uint32_t table_size_;
  uint32_t table_mask_;
  uint64_t correct_predictions_ = 0;
  uint64_t total_branches_ = 0;
  uint64_t taken_branches_ = 0;
  uint64_t correct_taken_ = 0;
  uint64_t correct_not_taken_ = 0;

  static constexpr uint32_t HASH_SHIFT = 2;

  uint32_t get_index(uint64_t branch_pc) const;

public:
  static constexpr uint32_t DEFAULT_TABLE_SIZE = 1024;
  explicit OneBitPredictor(uint32_t table_size = DEFAULT_TABLE_SIZE);

  bool predict(uint64_t branch_pc) const override;
  void update(uint64_t branch_pc, bool actual_outcome) override;

  double get_accuracy() const override;
  double get_taken_accuracy() const override;
  double get_not_taken_accuracy() const override;

  void reset_stats() override;
  void reset_all() override;

  std::string get_name() const override { return "One-Bit Predictor"; }
  void print_stats() const override;
};
