#include "../include/OneBitPredictor.h"
#include <iomanip>
#include <iostream>
#include <stdexcept>

OneBitPredictor::OneBitPredictor(uint32_t size) {
  if (size == 0) {
    throw std::invalid_argument("Table size must be at least 1");
  }

  // Round up to next power of 2
  table_size_ = 1;
  while (table_size_ < size) {
    table_size_ <<= 1;
  }

  table_mask_ = table_size_ - 1;
  prediction_table_.assign(table_size_, false);
  reset_stats();
}

uint32_t OneBitPredictor::get_index(uint64_t branch_pc) const {
  // Hash to reduce collisions
  return (branch_pc ^ (branch_pc >> HASH_SHIFT)) & table_mask_;
}

bool OneBitPredictor::predict(uint64_t branch_pc) const {
  // Predict based on last outcome stored in table
  return prediction_table_[get_index(branch_pc)];
}

void OneBitPredictor::update(uint64_t branch_pc, bool actual_outcome) {
  uint32_t index = get_index(branch_pc);
  bool prediction = prediction_table_[index];

  // Update statistics
  total_branches_++;
  if (actual_outcome) {
    taken_branches_++;
    if (prediction) {
      correct_taken_++;
    }
  } else if (!prediction) {
    correct_not_taken_++;
  }

  if (prediction == actual_outcome) {
    correct_predictions_++;
  }

  // Update table with actual outcome
  prediction_table_[index] = actual_outcome;
}

double OneBitPredictor::get_accuracy() const {
  return total_branches_
             ? static_cast<double>(correct_predictions_) / total_branches_
             : 0.0;
}

double OneBitPredictor::get_taken_accuracy() const {
  return taken_branches_ ? static_cast<double>(correct_taken_) / taken_branches_
                         : 0.0;
}

double OneBitPredictor::get_not_taken_accuracy() const {
  uint64_t not_taken = total_branches_ - taken_branches_;
  return not_taken ? static_cast<double>(correct_not_taken_) / not_taken : 0.0;
}

void OneBitPredictor::reset_stats() {
  correct_predictions_ = 0;
  total_branches_ = 0;
  taken_branches_ = 0;
  correct_taken_ = 0;
  correct_not_taken_ = 0;
}

void OneBitPredictor::reset_all() {
  reset_stats();
  std::fill(prediction_table_.begin(), prediction_table_.end(), false);
}

void OneBitPredictor::print_stats() const {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << get_name() << " (Table Size: " << table_size_ << ")\n";
  std::cout << "--------------------------------\n";
  std::cout << "Total Branches: " << total_branches_ << "\n";
  std::cout << "Overall Accuracy: " << get_accuracy() * 100 << "%\n";
  std::cout << "Taken Accuracy: " << get_taken_accuracy() * 100 << "% ("
            << taken_branches_ << " branches)\n";
  std::cout << "Not Taken Accuracy: " << get_not_taken_accuracy() * 100
            << "%\n";
  std::cout << "--------------------------------\n";
}
