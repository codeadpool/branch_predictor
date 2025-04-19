# branch-predictors
├── include/                          # Header files
│   ├── branch_predictor.h            # Abstract base class
│   ├── always_taken_predictor.h      # Always Taken predictor
│   ├── one_bit_predictor.h           # One-Bit predictor
│   ├── two_bit_predictor.h           # Two-Bit predictor
│   ├── gshare_predictor.h            # GShare predictor
│   ├── tournament_predictor.h        # Tournament predictor
│   └── trace_reader.h                # Trace file reader
├── src/                              # Source files
│   ├── always_taken_predictor.cpp    # Always Taken implementation
│   ├── one_bit_predictor.cpp         # One-Bit implementation
│   ├── two_bit_predictor.cpp         # Two-Bit implementation
│   ├── gshare_predictor.cpp          # GShare implementation
│   ├── tournament_predictor.cpp      # Tournament implementation
│   ├── trace_reader.cpp              # Trace reader implementation
│   └── main.cpp                      # Program entry point
├── tests/                            # Unit tests
│   ├── test_predictors.cpp           # Test cases
│   └── test_main.cpp                 # Test driver
├── traces/                           # Sample trace files
│   ├── t_trace.txt                   # Trace file
│   └── ...
├── build/                            # Build artifacts
│   ├── bin/
│   │   └── branch_predictor          # Executable
│   └── obj/                          # Object files
├── CMakeLists.txt                    # Build configuration
├── README.md                         # Project documentation
