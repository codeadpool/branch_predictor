cmake_minimum_required(VERSION 3.10)
project(BranchPredictor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(branch_predictor ${SOURCES})

# Optional: Add tests (uncomment to enable)
# find_package(GTest)
# if(GTEST_FOUND)
#     enable_testing()
#     file(GLOB_RECURSIVE TEST_SOURCES "tests/*.cpp")
#     add_executable(test_branch_predictor ${TEST_SOURCES} src/always_taken_predictor.cpp src/one_bit_predictor.cpp src/two_bit_predictor.cpp src/gshare_predictor.cpp src/tournament_predictor.cpp src/trace_reader.cpp)
#     target_link_libraries(test_branch_predictor GTest::GTest GTest::Main)
#     add_test(NAME BranchPredictorTests COMMAND test_branch_predictor)
# endif()
