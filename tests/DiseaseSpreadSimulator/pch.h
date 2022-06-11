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

// Extern libs
#include "nlohmann/json.hpp"

// Includes to be tested
// DiseaseSpreadSimulator
#include "DiseaseSpreadSimulator/IDGenerator/IDGenerator.h"
#include "DiseaseSpreadSimulator/RandomNumbers.h"
#include "DiseaseSpreadSimulator/Enums.h"
#include "DiseaseSpreadSimulator/RandomNumbers.h"
#include "DiseaseSpreadSimulator/Statistics.h"
#include "DiseaseSpreadSimulator/Disease/Disease.h"
#include "DiseaseSpreadSimulator/Disease/DiseaseBuilder.h"
#include "DiseaseSpreadSimulator/Disease/Infection.h"
#include "DiseaseSpreadSimulator/Person/Person.h"
#include "DiseaseSpreadSimulator/Person/PersonPopulator.h"
#include "DiseaseSpreadSimulator/Person/PersonBehavior.h"
#include "DiseaseSpreadSimulator/Simulation/TimeManager.h"
#include "DiseaseSpreadSimulator/Places/Places.h"
#include "DiseaseSpreadSimulator/Places/PlaceBuilder.h"
#include "DiseaseSpreadSimulator/Places/Community.h"
