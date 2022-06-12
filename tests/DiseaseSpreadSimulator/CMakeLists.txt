cmake_minimum_required(VERSION 3.23)

find_package(fmt CONFIG REQUIRED)

set(SIMULATORSOURCES
    test.cpp
    CommunityTests.cpp
    DiseaseContainmentTests.cpp
    DiseaseTests.cpp
    InfectionTests.cpp
    PersonTests.cpp
    PlaceTests.cpp
    TimeTests.cpp
)

add_executable(unit_tests_simulator ${SIMULATORSOURCES})

target_include_directories(unit_tests_simulator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(unit_tests_simulator
PUBLIC
  project_options
  project_warnings
PRIVATE
  ${CMAKE_PROJECT_NAME}
  gtest_main
  fmt::fmt
)

# automatic discovery of unit tests
gtest_discover_tests(unit_tests_simulator
)
