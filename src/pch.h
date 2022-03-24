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
#include <shared_mutex>
#include <execution>
#include <optional>

// Data structures
#include <string>
#include <vector>
#include <array>

// GSL: Guidelines Support Library
//#include <gsl/gsl>
 
// Extern libs
#include "fmt/core.h"

// My
#include "DiseaseSpreadSimulator/Enums.h"
#include "DiseaseSpreadSimulator/RandomNumbers.h"
#include "DiseaseSpreadSimulator/Statistics.h"
#include "Disease/Disease.h"
#include "Disease/Infection.h"
#include "Person/Person.h"
#include "Places/Places.h"
#include "Places/Community.h"
#include "Simulation/MeasureTime.h"
