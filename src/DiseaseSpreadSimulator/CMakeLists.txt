cmake_minimum_required(VERSION 3.22)

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
  # Places
  Places/Community.cpp
  Places/PlaceBuilder.cpp
  Places/Places.cpp
  # Simulation
  Simulation/MeasureTime.cpp
  Simulation/Simulation.cpp
  Simulation/TimeManager.cpp
)

set(HEADERS
  # Disease
  Disease/Disease.h
  Disease/DiseaseBuilder.h
  Disease/DiseaseContainment.h
  Disease/Infection.h
  # IDGenerator
  IDGenerator/IDGenerator.h
  # Person
  Person/Person.h
  Person/PersonBehavior.h
  Person/PersonPopulator.h
  # Places
  Places/Community.h
  Places/PlaceBuilder.h
  Places/Places.h
  # Simulation
  Simulation/MeasureTime.h
  Simulation/Simulation.h
  Simulation/TimeManager.h
  # Other
  Enums.h
  RandomNumbers.h
  Statistics.h
)

# Add a library with the above sources
add_library(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

# Add precompiled headers
if(OPT_ENABLE_PCH)
  message("-- Building ${CMAKE_PROJECT_NAME} with PCH")
  target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE pch.h)
endif()

# Add the current folder to the include directories
target_include_directories(${CMAKE_PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:>
)

# Link the external libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
  PUBLIC
    project_options
    project_warnings
)
target_link_system_libraries(${CMAKE_PROJECT_NAME}
  PUBLIC
    nlohmann_json::nlohmann_json
    fmt::fmt
)

# Add a executable with terminal output
add_executable("${CMAKE_PROJECT_NAME}Terminal" DiseaseSpreadSimulator.cpp)

# Add precompiled headers
if(OPT_ENABLE_PCH)
  message("-- Building ${CMAKE_PROJECT_NAME}Terminal with PCH")
  target_precompile_headers("${CMAKE_PROJECT_NAME}Terminal" REUSE_FROM ${CMAKE_PROJECT_NAME})
endif()

target_include_directories("${CMAKE_PROJECT_NAME}Terminal" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  "${CMAKE_PROJECT_NAME}Terminal"
  PUBLIC
    project_options
    project_warnings
  PRIVATE
    ${CMAKE_PROJECT_NAME}
)
