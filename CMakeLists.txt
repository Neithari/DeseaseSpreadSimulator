cmake_minimum_required(VERSION 3.22)

# For debugging: Print variables with "cmake_print_variables(MY_VARIABLE)"
include(CMakePrintHelpers)

# Not ideal to use this global variable, but necessary to make sure
# that tooling and projects use the same version
set(CMAKE_CXX_STANDARD 20)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS ON)

# Note: by default ENABLE_DEVELOPER_MODE is True
# This means that all analysis (sanitizers, static analysis)
# is enabled and all warnings are treated as errors
# if you want to switch this behavior, change TRUE to FALSE
set(ENABLE_DEVELOPER_MODE
    TRUE
    CACHE BOOL "Enable 'developer mode'"
)

# Change this to false if you want to disable warnings_as_errors in developer mode
set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)

# Add project_options v0.21.1
# https://github.com/cpp-best-practices/project_options
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.21.1.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)
# Add dynamic CMake options
include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)

# Enable vcpkg:
# Setup VCPKG_ROOT per command line or in a CMakeUserPresets.json
# Will be installed in HOME when no VCPKG_ROOT is provided
run_vcpkg(ENABLE_VCPKG_UPDATE TRUE VCPKG_DIR "${VCPKG_ROOT}")

# Set the project name and language
project(
  DiseaseSpreadSimulator
  VERSION 0.0.1
  DESCRIPTION "Simulating the spread of a disease"
  LANGUAGES CXX
)

# defaulted_project_options sets recommended defaults and provides user and developer
# modes and full GUI support for choosing options at configure time

# for more flexibility, look into project_options() macro

# Any default can be overridden
# set(<feature_name>_DEFAULT <value>) - set default for both user and developer modes
# set(<feature_name>_DEVELOPER_DEFAULT <value>) - set default for developer mode
# set(<feature_name>_USER_DEFAULT <value>) - set default for user mode

# Set PCH to be on by default for all non-Developer Mode Builds
set(ENABLE_PCH_DEVELOPER_DEFAULT OFF)
set(ENABLE_PCH_USER_DEFAULT ON)
set(ENABLE_CLANG_TIDY_DEFAULT OFF)

# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment the options to enable them:
dynamic_project_options(
  # Note: PCH is disabled by default in developer mode because these headers become
  # globally included and they can mask other errors
  # ENABLE_PCH
  PCH_HEADERS
  # ENABLE_CONAN
  # CONAN_OPTIONS    # Extra options to pass to conan
  # MSVC_WARNINGS    # Override the defaults for the MSVC warnings
  # CLANG_WARNINGS   # Override the defaults for the CLANG warnings
  # GCC_WARNINGS     # Override the defaults for the GCC warnings
  CPPCHECK_OPTIONS
  --enable=style,performance,warning,portability
  --inline-suppr
  # We cannot act on a bug/missing feature of cppcheck
  --suppress=cppcheckError
  --suppress=internalAstError
  # if a file does not have an internalAstError, we get an unmatchedSuppression error
  --suppress=unmatchedSuppression
  --suppress=passedByValue
  --suppress=syntaxError
  --inconclusive
  # suppress all google test files
  --suppress=*:*gtest/*
)

target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
# TODO: The INTERFACE library NAMESPACE ALIAS are missing! CK
add_library(${CMAKE_PROJECT_NAME}::project_options INTERFACE IMPORTED)
add_library(${CMAKE_PROJECT_NAME}::project_warnings INTERFACE IMPORTED)

# Adding the src:
add_subdirectory(src)

# Adding the tests:
option(ENABLE_TESTING "Enable the tests" ${PROJECT_IS_TOP_LEVEL})
if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

# If MSVC is being used, and ASAN is enabled, we need to set the debugger environment
# so that it behaves well with MSVC's debugger, and we can run the target from visual studio
if(MSVC)
  get_all_installable_targets(all_targets)
  message("all_targets=${all_targets}")
  set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

if(CMAKE_SKIP_INSTALL_RULES)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()
