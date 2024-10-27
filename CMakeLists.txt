cmake_minimum_required(VERSION 3.10)
project(ADBIDDER)

set(CMAKE_CXX_STANDARD 17)

# Include Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

# Add source files
add_library(auction_lib src/advertiser.hpp src/adslot.hpp src/auction_engine.hpp src/lock_free_ring_buffer.hpp)

# Main executable (optional)
add_executable(main_exec src/main.cpp)
target_link_libraries(main_exec auction_lib)

# Tests
enable_testing()
add_executable(run_tests tests/test_main.cpp tests/advertiser_test.cpp tests/adslot_test.cpp tests/auction_engine_test.cpp)
target_link_libraries(run_tests auction_lib gtest gtest_main)
add_test(NAME run_tests COMMAND run_tests)
