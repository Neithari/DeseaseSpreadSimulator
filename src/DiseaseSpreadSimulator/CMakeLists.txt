cmake_minimum_required(VERSION 3.23)

find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(SOURCES
  # Disease
  Disease/Disease.cpp
  Disease/DiseaseBuilder.cpp
  Disease/DiseaseContainment.cpp
  Disease/Infection.cpp
  # IDGenerator
  IDGenerator/IDGenerator.cpp
  # Person
  Person/Person.cpp
  Person/PersonBehavior.cpp
  Person/PersonPopulator.cpp
  #Places
  Places/Community.cpp
  Places/PlaceBuilder.cpp
  Places/Places.cpp
  # Simulation
  Simulation/MeasureTime.cpp
  Simulation/Simulation.cpp
  Simulation/TimeManager.cpp
)

# Add a library with the above sources
add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCES})

# Add the current folder to the include directories
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:>
)

# Set the c++ version to 20 
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)

# Link the external libraries
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  PUBLIC
    project_options
    project_warnings
  PRIVATE
    fmt::fmt
    nlohmann_json::nlohmann_json
)

# Add a executable with terminal output
add_executable("${CMAKE_PROJECT_NAME}Terminal" DiseaseSpreadSimulator.cpp)

target_include_directories("${CMAKE_PROJECT_NAME}Terminal" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  "${CMAKE_PROJECT_NAME}Terminal"
  PUBLIC
    project_options
    project_warnings
  PRIVATE
    ${CMAKE_PROJECT_NAME}
    fmt::fmt
)
