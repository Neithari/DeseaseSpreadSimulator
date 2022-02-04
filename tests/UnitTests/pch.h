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

// Includes to be tested
#include "DeseaseSpreadSimulator/Enums.h"
#include "Desease/Desease.h"
#include "Desease/DeseaseBuilder.h"
#include "Person/Person.h"
#include "Person/PersonPopulator.h"
#include "Person/PersonBehavior.h"
#include "Simulation/TimeManager.h"
#include "IDGenerator/IDGenerator.h"
#include "Places/Places.h"
#include "Places/Community.h"
#include "Places/CommunityBuilder.h"
#include "DeseaseSpreadSimulator/Statistics.h"
