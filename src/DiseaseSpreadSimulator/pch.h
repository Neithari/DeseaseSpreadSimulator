// Precompiled headers go in here
#pragma once

// Standard Library Header
#include <cstdint>
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
#include <iostream>
#include <cmath>

// Data structures
#include <string>
#include <vector>
#include <array>

// Extern libs
#include "fmt/core.h"
#include "nlohmann/json.hpp"

// My
#include "Enums.h"
#include "RandomNumbers.h"
#include "Statistics.h"
#include "IDGenerator/IDGenerator.h"
#include "Disease/Disease.h"
#include "Disease/Infection.h"
#include "Person/Person.h"
#include "Places/Places.h"
#include "Places/Community.h"
#include "Simulation/MeasureTime.h"
