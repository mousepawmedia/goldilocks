# CMake Config (MousePaw Media Build System)
# Version: 3.2.1

# CHANGE: Name your project here
project("Goldilocks")

# Specify the version being used.
cmake_minimum_required(VERSION 3.8)

# Import user-specified library path configuration
message("Using ${CONFIG_FILENAME}.config")
include(${CMAKE_HOME_DIRECTORY}/../${CONFIG_FILENAME}.config)

# CHANGE: Specify output binary name
set(TARGET_NAME "goldilocks")

# SELECT: Project artifact type
set(ARTIFACT_TYPE "library")
#set(ARTIFACT_TYPE "executable")

# CHANGE: Find dynamic library dependencies.

# CHANGE: Include headers of dependencies.
set(INCLUDE_LIBS
    ${IOSQUEAK_DIR}/include
    ${ARCTICTERN_DIR}/include
    ${EVENTPP_DIR}/include
)

# CHANGE: Include files to compile.
set(FILES
    include/goldilocks/expect/expect.hpp
    include/goldilocks/expect/outcomes.hpp
    include/goldilocks/expect/should.hpp
    include/goldilocks/expect/that.hpp

    include/goldilocks/benchmark_results.hpp
    include/goldilocks/benchmarker.hpp
    include/goldilocks/clock.hpp
    include/goldilocks/runner.hpp
    include/goldilocks/test.hpp
    include/goldilocks/types.hpp

    src/benchmarker.cpp
    src/runner.cpp
)

# CHANGE: Link against dependencies.
set(LINK_LIBS
    ${IOSQUEAK_DIR}/lib/libiosqueak.a
)

# Imports build script. (Change if necessary to point to build.cmake)
include(${CMAKE_HOME_DIRECTORY}/../build_system/build.cmake)
