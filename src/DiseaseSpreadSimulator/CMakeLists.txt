find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(SOURCES
    # Main
    DiseaseSpreadSimulator.cpp
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
# Add the current folder to the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add an executable with the above sources
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Link the external libraries
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  PUBLIC project_options project_warnings
  PRIVATE fmt::fmt nlohmann_json::nlohmann_json)
