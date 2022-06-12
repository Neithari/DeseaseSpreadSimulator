//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "gtest/gtest.h"

// Used during testing
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <numeric>

// Extern libs
#include "nlohmann/json.hpp"

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
