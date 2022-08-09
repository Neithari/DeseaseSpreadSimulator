cmake_minimum_required(VERSION 3.22)

# For debugging: Print variables with "cmake_print_variables(MY_VARIABLE)"
include(CMakePrintHelpers)

# set a default CXX standard for the tools and targets that do not specify them.
# If commented, the latest supported standard for your compiler is automatically set.
# set(CMAKE_CXX_STANDARD 20)

# Note: by default ENABLE_DEVELOPER_MODE is True
# This means that all analysis (sanitizers, static analysis)
# is enabled and all warnings are treated as errors
# if you want to switch this behavior, change TRUE to FALSE
set(ENABLE_DEVELOPER_MODE
  TRUE
  CACHE BOOL "Enable 'developer mode'"
)

# Change this to "" if you want to disable warnings_as_errors in developer mode
if(ENABLE_DEVELOPER_MODE)
  set(WARNINGS_AS_ERRORS "WARNINGS_AS_ERRORS")
endif()

# Add project_options v0.24.0
# https://github.com/aminya/project_options
# Change the version in the following URL to update the package (watch the releases of the repository for future updates)
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.24.0.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

if(MSVC)
  msvc_toolchain()
endif()

# Enable vcpkg:
# Setup VCPKG_ROOT per command line or in a CMakeUserPresets.json
# Will be installed in HOME when no VCPKG_ROOT is provided
run_vcpkg(ENABLE_VCPKG_UPDATE TRUE VCPKG_DIR "${VCPKG_ROOT}")

# Set the project name and language
project(
  DiseaseSpreadSimulator
  VERSION 0.1.3
  DESCRIPTION "Simulating the spread of a disease"
  LANGUAGES CXX
)

option(ENABLE_PCH "Enable PCH Usage" OFF)
# Enable PCH in user mode
if(NOT ENABLE_DEVELOPER_MODE)
  set(ENABLE_PCH ON)
endif()

# Enable sanitizers and clang-tidy if running the tests
set(ENABLE_CLANG_TIDY OFF)
set(ENABLE_CPPCHECK OFF)
set(ENABLE_SANITIZER_ADDRESS OFF)
set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR OFF)
set(ENABLE_INCLUDE_WHAT_YOU_USE OFF)

option(ENABLE_TESTING "Enable the tests" ${PROJECT_IS_TOP_LEVEL})

if(ENABLE_TESTING)
  # MSVC and microsoft clang are producing too many false positives so we keep it disabled
  if(NOT MSVC)
    # Clang-tidy only works with clang when PCH is enabled
    if(${ENABLE_PCH} AND (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
      message(${WARNING_MESSAGE}
        "Clang-tidy can't be enabled with non-clang compiler and PCH, it can't handle gcc's PCH file. Disabling PCH..."
      )
      set(ENABLE_PCH OFF)
    endif()

    set(ENABLE_CLANG_TIDY "ENABLE_CLANG_TIDY")
  endif()

  set(ENABLE_CPPCHECK "ENABLE_CPPCHECK")

  if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
    set(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "ENABLE_SANITIZER_UNDEFINED_BEHAVIOR")
    set(ENABLE_INCLUDE_WHAT_YOU_USE "ENABLE_INCLUDE_WHAT_YOU_USE")
  else()
    # or it is MSVC and has run vcvarsall
    string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)

    if(MSVC AND "${index_of_vs_install_dir}" STREQUAL "-1")
      set(ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
    endif()
  endif()
endif()

# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment to enable the options. Some of them accept one or more inputs:
project_options(
  ENABLE_CACHE
  ${ENABLE_CPPCHECK}
  ${ENABLE_CLANG_TIDY}
  # ENABLE_INTERPROCEDURAL_OPTIMIZATION
  # ENABLE_NATIVE_OPTIMIZATION
  # ${ENABLE_DOXYGEN}
  # ${ENABLE_COVERAGE}
  ${ENABLE_SANITIZER_ADDRESS}
  ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
  # ENABLE_SANITIZER_LEAK
  # ENABLE_SANITIZER_THREAD
  # ENABLE_SANITIZER_MEMORY
  # ENABLE_PCH
  # PCH_HEADERS
  ${WARNINGS_AS_ERRORS}
  ${ENABLE_INCLUDE_WHAT_YOU_USE}
  # ENABLE_USER_LINKER
  # ENABLE_BUILD_WITH_TIME_TRACE
  # ENABLE_UNITY
)

target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
add_library(${CMAKE_PROJECT_NAME}::project_options INTERFACE IMPORTED)
add_library(${CMAKE_PROJECT_NAME}::project_warnings INTERFACE IMPORTED)

# Adding the src:
add_subdirectory(src)

# Adding the tests:
if(ENABLE_TESTING)
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
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "${CMAKE_PROJECT_NAME}Terminal")

if(CMAKE_SKIP_INSTALL_RULES)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  return()
endif()
