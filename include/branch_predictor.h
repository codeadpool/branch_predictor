#pragma once

#include <cstdint>
#include <string>

class BranchPredictor {
public:
  virtual ~BranchPredictor() = default;

  virtual bool predict(uint64_t branch_pc) const = 0;
  virtual void update(uint64_t branch_pc, bool actual_outcome) = 0;

  virtual double get_accuracy() const = 0;
  virtual double get_taken_accuracy() const = 0;
  virtual double get_not_taken_accuracy() const = 0;

  virtual void reset_stats() = 0;
  virtual void reset_all() = 0;

  virtual std::string get_name() const = 0;
  virtual void print_stats() const = 0;
};
