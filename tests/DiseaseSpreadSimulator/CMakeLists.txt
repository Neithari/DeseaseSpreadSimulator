cmake_minimum_required(VERSION 3.22)

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
set(UNIT_TEST_NAME "unit_tests_simulator")
add_executable(${UNIT_TEST_NAME} ${SIMULATORSOURCES})

target_include_directories(${UNIT_TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add precompiled headers
if(OPT_ENABLE_PCH)
  message("-- Building ${UNIT_TEST_NAME} with PCH")
  target_precompile_headers(${UNIT_TEST_NAME} PRIVATE pch.h)
endif()

target_link_libraries(${UNIT_TEST_NAME}
PUBLIC
  project_options
  project_warnings
PRIVATE
  ${CMAKE_PROJECT_NAME}
)
target_link_system_libraries(${UNIT_TEST_NAME} PRIVATE google_test)

# automatic discovery of unit tests
gtest_discover_tests(unit_tests_simulator)
