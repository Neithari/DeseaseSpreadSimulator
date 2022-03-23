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

// Includes to be tested
#include "DiseaseSpreadSimulator/Enums.h"
#include "DiseaseSpreadSimulator/RandomNumbers.h"
#include "DiseaseSpreadSimulator/Statistics.h"
#include "Disease/Disease.h"
#include "Disease/DiseaseBuilder.h"
#include "Disease/Infection.h"
#include "Person/Person.h"
#include "Person/PersonPopulator.h"
#include "Person/PersonBehavior.h"
#include "Simulation/TimeManager.h"
#include "IDGenerator/IDGenerator.h"
#include "Places/Places.h"
#include "Places/PlaceBuilder.h"
#include "Places/Community.h"
