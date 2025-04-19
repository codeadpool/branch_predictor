#include "../include/static_bp.h"
#include <iomanip>
#include <iostream>

StaticBranchPredictor::StaticBranchPredictor(bool always_taken)
    : always_taken_(always_taken) {
  reset_stats();
}

bool StaticBranchPredictor::predict(uint64_t /*branch_pc*/) const {
  return always_taken_;
}

void StaticBranchPredictor::update(uint64_t /*branch_pc*/,
                                   bool actual_outcome) {
  total_branches_++;
  if (actual_outcome) {
    taken_branches_++;
    if (always_taken_) {
      correct_taken_++;
      correct_predictions_++;
    }
  } else {
    if (!always_taken_) {
      correct_not_taken_++;
      correct_predictions_++;
    }
  }
}

double StaticBranchPredictor::get_accuracy() const {
  return total_branches_
             ? static_cast<double>(correct_predictions_) / total_branches_
             : 0.0;
}

double StaticBranchPredictor::get_taken_accuracy() const {
  return taken_branches_ ? static_cast<double>(correct_taken_) / taken_branches_
                         : 0.0;
}

double StaticBranchPredictor::get_not_taken_accuracy() const {
  uint64_t not_taken = total_branches_ - taken_branches_;
  return not_taken ? static_cast<double>(correct_not_taken_) / not_taken : 0.0;
}

void StaticBranchPredictor::reset_stats() {
  correct_predictions_ = 0;
  total_branches_ = 0;
  taken_branches_ = 0;
  correct_taken_ = 0;
  correct_not_taken_ = 0;
}

void StaticBranchPredictor::reset_all() { reset_stats(); }

void StaticBranchPredictor::print_stats() const {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << get_name() << "\n";
  std::cout << "--------------------------------\n";
  std::cout << "Total Branches: " << total_branches_ << "\n";
  std::cout << "Overall Accuracy: " << get_accuracy() * 100 << "%\n";
  std::cout << "Taken Accuracy: " << get_taken_accuracy() * 100 << "% ("
            << taken_branches_ << " branches)\n";
  std::cout << "Not Taken Accuracy: " << get_not_taken_accuracy() * 100
            << "%\n";
  std::cout << "--------------------------------\n";
}
