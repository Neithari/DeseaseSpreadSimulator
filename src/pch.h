// Precompiled headers go in here
#pragma once

// Disable/drop warnings for STL and library headers 
#ifdef _MSC_VER
#pragma warning(disable: 4290) // Suppress 'C++ Exception Specification ignored'
#pragma warning(disable: 4710) // Suppress 'function ... not inlined' for Release builds
#pragma warning(disable: 4514) // Suppress '... unreferenced inline function has been removed'
#pragma warning(disable: 4786) // Suppress '... truncated to 255 chars in debug'
#pragma warning(push, 3)       // Set warning levels to a quieter level for the STL
#endif

#include <iostream>
#include <fstream>
#include <memory>
#include <list>
#include <chrono>
#include <stdexcept>
#include <random>
#include <algorithm>

// Data structures
#include <string>
#include <vector>
#include <thread>
#include <coroutine>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#ifdef _MSC_VER
#pragma warning(pop)			// Restore warning levels for our code
#endif

// My