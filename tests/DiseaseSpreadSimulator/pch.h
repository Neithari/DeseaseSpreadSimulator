// Precompiled headers go in here
#pragma once

// Google test
#include <gtest/gtest.h>

// Used during testing
#include <cstdint>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <optional>
#include <utility>
#include <thread>
#include <shared_mutex>
#include <stdexcept>

// Extern libs
#include <nlohmann/json.hpp>

// Includes to be tested
// DiseaseSpreadSimulator
#include "IDGenerator/IDGenerator.h"
#include "RandomNumbers.h"
#include "Enums.h"
#include "RandomNumbers.h"
#include "Statistics.h"
#include "Disease/Disease.h"
#include "Disease/DiseaseBuilder.h"
#include "Disease/Infection.h"
#include "Person/Person.h"
#include "Person/PersonPopulator.h"
#include "Person/PersonBehavior.h"
#include "Simulation/TimeManager.h"
#include "Places/Places.h"
#include "Places/PlaceBuilder.h"
#include "Places/Community.h"
