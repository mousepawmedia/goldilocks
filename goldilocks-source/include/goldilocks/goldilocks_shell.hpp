/** Goldilocks Shell [Goldilocks]
 * Version: 2.0
 *
 * Base class for Goldilocks suites, which are collections of tests.
 *
 * Author(s): Jack Tolmie
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2023 MousePaw Media.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * CONTRIBUTING
 * See https://www.mousepawmedia.com/developers for information
 * on how to contribute to our projects.
 */
#ifndef GOLDILOCKS_SHELL_HPP
#define GOLDILOCKS_SHELL_HPP

#include "iosqueak/blueshell.hpp"
#include "goldilocks/coordinator/coordinator.hpp"

#include <algorithm>
#include <iostream>
#include <string_view>

class Goldilocks_Shell: private Blueshell
{
 
private:

    /* Coordinator object for accessing the tree. */
    Coordinator coordinate;

    /* information about the a specific test */
    int about(Alias::commands);
    
    /* Runs a benchmark on [test], using its registered 
     * comparative, with (number) repetitions. */
    int benchmark(Alias::commands);
    
    /* Function to check if the sent arguments and/or
     * options are valid. */
    bool check_tests(Alias::commands, const std::string& test_name = __builtin_FUNCTION());
    
    /* Runs a comparative benchmark between [test1] and 
     * [test2] with (number) repetitions. This is seldom
     * used, as its prone to apples-and-oranges comparisons,
     * unlike benchmark, but it can be useful at times. */
    int compare(Alias::commands);
    
    /* Function to register find_node on the shell. Used to
     * find tests that are currently loaded. */
    int find(Alias::commands);

    /* Testing for printing Suites containers. Delete of not
     * working or needed.*/
    // void get_suites(std::vector<Suites*>&);
    void get_suites(Suites*);
    
    /* Displays the available suites. Passing an optional
     * (suite) will display all the tests in that suite. */
    int list (Alias::commands);
    
    /* Function that will display all the loaded Suites
     * and Tests. */
    int list_loaded(Alias::commands);
    
    /* Function to register load_node test to the shell.
     * Used to load all the suites, or the suites passed
     * as arguments. */
    int load(Alias::commands);
    
    /* Runs an [item], being a test or suite, with the
     * optional (number) of repetitions, default 1. */
    int run(Alias::commands);
    
    /* Runs a [test] as a "controlled crash", logging test
     * details to an external file on the way down. This is
     * the only way to run Fatality Tests, but is also
     * useful for Stress Tests, Edge Tests, and Fuzz Test
     * when a crash is occurring. */
    int kamikaze(Alias::commands);
    
    /* Register default commands to add to the container
     * of available commands. */
    void register_defaults();
 
public:
    explicit Goldilocks_Shell(const std::string&);
    virtual ~Goldilocks_Shell() = default;
};

#endif  // GOLDILOCKS_SHELL_HPP
