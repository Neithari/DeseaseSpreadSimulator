cmake_minimum_required(VERSION 3.22)

enable_testing()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_library(google_test INTERFACE)
target_link_libraries(google_test
  INTERFACE
    Threads::Threads
    GTest::gmock
    GTest::gtest
    GTest::gmock_main
    GTest::gtest_main
)

# Unittests for DiseaseSpreadSimulator
add_subdirectory(DiseaseSpreadSimulator)
