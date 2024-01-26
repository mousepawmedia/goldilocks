# CMake Config (MousePaw Media Build System)
# Version: 3.2.1

# CHANGE: Name your project here
project("Goldilocks")

# Specify the version being used.
cmake_minimum_required(VERSION 3.8)

# Set C++ Version.
set(CMAKE_CXX_STANDARD 17)

# Import user-specified library path configuration
message("Using ${CONFIG_FILENAME}.config")
#include(${CMAKE_HOME_DIRECTORY}/../${CONFIG_FILENAME}.config)

# Changed by Jack for running in QT Creator. Delete when done.
include(${CMAKE_HOME_DIRECTORY}/../default.config)

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
    include/goldilocks/report.hpp
    include/goldilocks/runner.hpp
    include/goldilocks/suite.hpp
    include/goldilocks/test.hpp
    include/goldilocks/types.hpp

    src/benchmarker.cpp
    src/suite.cpp
    src/benchmark_results.cpp
    
# For Coordinator
    include/goldilocks/coordinator/alias.hpp
    include/goldilocks/coordinator/coordinator.hpp
    include/goldilocks/coordinator/node.hpp
    include/goldilocks/coordinator/runnable.hpp
    include/goldilocks/coordinator/suites.hpp
    include/goldilocks/coordinator/tests.hpp

    src/coordinator/add_find.cpp
    src/coordinator/add_node.cpp
    src/coordinator/coordinator.cpp
    src/coordinator/node.cpp
    src/coordinator/process_tests.cpp
    src/coordinator/suites.cpp
    src/coordinator/tests.cpp

    #    src/coordinator/add_children.cpp
    #
    #    src/coordinator/add_suite.cpp
    #    src/coordinator/find_child.cpp
    #    src/coordinator/find_node.cpp
    
# For Goldilocks Shell.
    include/goldilocks/goldilocks_shell.hpp

    src/goldilocks_shell/about.cpp
    src/goldilocks_shell/benchmark.cpp
    src/goldilocks_shell/check_tests.cpp
    src/goldilocks_shell/compare.cpp
    src/goldilocks_shell/find.cpp
    src/goldilocks_shell/goldilocks_shell.cpp
    src/goldilocks_shell/kamikaze.cpp
    src/goldilocks_shell/list.cpp
    src/goldilocks_shell/list_loaded.cpp
    src/goldilocks_shell/load.cpp
    src/goldilocks_shell/registerdefaults.cpp
    src/goldilocks_shell/run.cpp
    
# For Suites/Tests. Delete when done.
#    include/goldilocks/coordinator/suites_tests/suite_first.hpp
#    include/goldilocks/coordinator/suites_tests/suite_main.hpp
#    include/goldilocks/coordinator/suites_tests/suite_second.hpp
#    include/goldilocks/coordinator/suites_tests/test_list.hpp
#    include/goldilocks/coordinator/suites_tests/test1.hpp
#    include/goldilocks/coordinator/suites_tests/test2.hpp
#    include/goldilocks/coordinator/suites_tests/test3.hpp
#    include/goldilocks/coordinator/suites_tests/test_main.hpp

#    src/suites_tests/suites_print.cpp
#    src/suites_tests/suites_in_suites/suite_second.cpp
#    src/suites_tests/suites_in_suites/suite_first.cpp
#    src/suites_tests/suites_in_suites/suite_main.cpp
#    src/suites_tests/test_list.cpp
#    src/suites_tests/test_main.cpp
#    src/suites_tests/test_suite1/test1.cpp
#    src/suites_tests/test_suite2/test2.cpp
#    src/suites_tests/test_suite3/test3.cpp
)

# CHANGE: Link against dependencies.
set(LINK_LIBS
    ${IOSQUEAK_DIR}/lib/libiosqueak.a
)

# Imports build script. (Change if necessary to point to build.cmake)
include(${CMAKE_HOME_DIRECTORY}/../build_system/build.cmake)

target_sources(goldilocks
  PRIVATE
    src/coordinator/add_find.cpp
)
