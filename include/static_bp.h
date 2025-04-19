#pragma once

#include "branch_predictor.h"
#include <cstdint>
#include <string>

class StaticBranchPredictor : public BranchPredictor {
private:
  bool always_taken_; // Strategy flag: true for always taken, false for always
                      // not taken
  uint64_t correct_predictions_ = 0;
  uint64_t total_branches_ = 0;
  uint64_t taken_branches_ = 0;
  uint64_t correct_taken_ = 0;
  uint64_t correct_not_taken_ = 0;

public:
  explicit StaticBranchPredictor(bool always_taken = true);

  bool predict(uint64_t branch_pc) const override;
  void update(uint64_t branch_pc, bool actual_outcome) override;

  double get_accuracy() const override;
  double get_taken_accuracy() const override;
  double get_not_taken_accuracy() const override;

  void reset_stats() override;
  void reset_all() override;

  std::string get_name() const override {
    return always_taken_ ? "Static Always Taken Predictor"
                         : "Static Always Not Taken Predictor";
  }
  void print_stats() const override;
};
