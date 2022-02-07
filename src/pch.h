// Precompiled headers go in here
#pragma once

// Standard Library Header
#include <format>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <utility>
#include <mutex>
#include <execution>

// Data structures
#include <string>
#include <vector>
#include <array>

// GSL: Guidelines Support Library
//#include <gsl/gsl>
 
// Extern libs
#include "fmt/core.h"

// My
#include "DeseaseSpreadSimulator/Enums.h"
#include "Desease/Desease.h"
#include "Person/Person.h"
#include "Places/Community.h"
#include "Places/Places.h"
#include "Simulation/MeasureTime.h"
