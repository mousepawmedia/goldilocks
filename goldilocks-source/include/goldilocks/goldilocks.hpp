/** Goldilocks Test System [PawLIB]
 * Version: 2.0
 *
 * Goldilocks stores, manages, and calls tests.
 * It also contains a runtime benchmarker which
 * measures CPU cycles for any given test with
 * as much accuracy as is possible from user-level
 * assembly code. Goldilocks' benchmarker also
 * calculates additional useful statistical data,
 * especially for comparing the execution of two
 * tests.
 *
 * Author(s): Jason C. McDonald, Wilfrantz DEDE
 */
/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2020 MousePaw Media.
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

#ifndef GOLDILOCKS_BENCHMARKER_HPP
#define GOLDILOCKS_BENCHMARKER_HPP

#include <stdexcept>
// uint64_t and friends
#include <cstdlib>
// std::unique_ptr
#include <memory>
// std::map (we will use this here)
#include <map>
// std::vector (we will use this here)
#include <vector>
#include <cstdint>

#include "goldilocks/types.hpp"
#include "goldilocks/runner.hpp"
#include "goldilocks/test.hpp"

class Test;
class TestSuite;
class TestManager;

/* The Test smart pointer type shall henceforth be known
    * as "testptr_t".*/
typedef std::unique_ptr<Test> testptr_t;
/* The TestSuite smart pointer type shall henceforth be known
    * as "testsuiteptr_t".*/
typedef std::unique_ptr<TestSuite> testsuiteptr_t;

/* The type we use for storing test names. */
typedef std::string testname_t;
/* The type we use for storing suite names. Alias of testname_t.*/
typedef testname_t testsuitename_t;
/* The type we use for storing documentation strings. Alias of testname_t.*/
typedef testname_t testdoc_t;

enum class GolidlocksItemType
{
    test,
    comparative,
    suite
};

class Benchmarker final : Runner
{
protected:
	Test* test;
	Test* comparative;
	uint16_t iterations;

	inline bool execute_pass(Test* on);
	bool die(bool test_ok, bool comparative_ok);
    
    // BenchmarkResult benchmarkResult[3]; 
    // BenchmarkResult result_mamaBear;
    // BenchmarkResult result_papaBear;
    // BenchmarkResult result_babyBear;
    /* The BenchmarkResult instances for storing the output of all our
        * measurements.*/
    BenchmarkResult mama1, mama2, papa1, papa2, baby1, baby2, baseR;
    
	
public:
    Benchmarker(Test* comparative, Test* test, uint16_t iterations = 1)
	: test(test), iterations(iterations), comparative(comparative)
	{}

	// Setup both tests to be run.
	void callPre();

	void warmup(); 

    void calibrate();
    
    // Run benchmark as "mama bear": 1 test, 1 comp, 1 test...
    void mamaBear();
    void papaBear();
    void babyBear(); 
    //TODO: Papa bear, baby bear, switch functions to return ints for 
    //      error checking
    // void papabear();
    // void babybear();

    /* void fullBenchmarker()
    {
        callPre();
        callibrate();
        mamaBear();
    }


    */
    ~Benchmarker() = default;

protected:
    /**The BenchmarkResult struct stores all of the statistical data
     * from a single test benchmark. Having this struct makes our
     * code more efficient, and allows us to write standard
     * functions for calculating results and comparisons, and
     * outputting everything.*/
    struct BenchmarkResult{
        // TODO
    };
	/**The BenchmarkResult struct stores all of the statistical data
	 * from a single test benchmark. Having this struct makes our
	 * code more efficient, and allows us to write standard
	 * functions for calculating results and comparisons, and
	 * outputting everything.*/

	/**Convert a raw array of clock measurements into a complete
	 * benchmark result. This does all of our statistical computations.
	 * \param the BenchmarkResult instance to write to
	 * \param the array of CPU cycle measurements, stored as
	 * 64-bit integers
	 * \param the size of the array (number of measurements)*/
	void resultFromArray(BenchmarkResult&, uint64_t arr[], int);

    /** Measure the number of CPU cycles it takes to execute a test
     * (or just the measurement itself, to get a baseline measurement).
     * To increase average accuracy, this does not serialize execution.
     * We are assuming that an out-of-order measurement will get removed
     * as an outlier in a set of measurements.
     * \param the raw pointer to the test to run. If omitted, no test
     * will be executed, allowing us to measure the execution time of
     * the measurement instructions themselves.
     * \return the CPU cycle measurement as a 64-bit unsigned integer*/
	inline uint64_t clock(Test*);
	
};

#endif // PAWLIB_GOLDILOCKS_HPP
