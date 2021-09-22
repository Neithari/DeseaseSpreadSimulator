//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "gtest/gtest.h"

// Used during testing
#include <chrono>
#include <thread>

// Includes to be tested
#include "Desease/Desease.h"
#include "Desease/DeseaseBuilder.h"
#include "Person/Person.h"
#include "Simulation/TimeManager.h"
#include "Person/PersonPopulator.h"
#include "IDGenerator/IDGenerator.h"
#include "Places/Places.h"
#include "Places/Community.h"
#include "Places/CommunityBuilder.h"
