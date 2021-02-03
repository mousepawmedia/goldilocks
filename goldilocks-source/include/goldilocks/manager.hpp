/** Manager [Goldilocks]
 * Version 1.3
 *
 * A ready-to-use interface for Goldilocks.
 *
 * Author(s): Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2021 MousePaw Media.
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

#ifndef GOLDILOCKS_MANAGER_HPP
#define GOLDILOCKS_MANAGER_HPP

// uint64_t and friends
#include <cstdlib>
// std::map
#include <map>

// Output
#include "iosqueak/channel.hpp"

#include "goldilocks/suite.hpp"
#include "goldilocks/test.hpp"
#include "goldilocks/types.hpp"

/**TestManager stores and calls tests by names. It exposes functions for
 * testing and benchmarking, and automatically handles the proper call
 * sequence for any Test object. It also handles garbage collection for
 * all tests under its control.*/
class TestManager
{
public:
	/* We are using smart pointers, so that TestManager exclusively
	 * owns all test instances it uses, and thus can delete them
	 * automatically later.*/

	/**The TestManager doesn't need anything to its constructor,
	 * as all of its tests will be added ("registered") after the
	 * fact, and it doesn't do any heap allocation besides that.*/
	TestManager() : tests(), suites(), comparatives() {}

	/**List all tests registered with the TestManager.
	 * \param whether to show the titles
	 */
	void list_tests(bool = true);

	/**List all suites registered with the TestManager.
	 * \param whether to show the titles
	 */
	void list_suites(bool = true);

	/** Load the tests in a test suite.
	 * \param the name of the suite to load (load all if unspecified)
	 * \return true if loaded, else false */
	bool load_suite(testsuitename_t = "");

	/**Get the documentation string for a given test.
	 * \param the test to return the documentation string for
	 * \return the documentation string.
	 */
	void show_docs(testname_t);

	/**Register a new test with the TestManager.
	 * \param a string of the test's name
	 * \param a pointer to the test. The safest way to use this is to
	 * pass the "new Test" as this argument, since TestManager will be
	 * taking exclusive ownership of the instance. It will handle
	 * NULL automatically, so no error checking is required.
	 * \param a pointer to the comparative test. See above.*/
	void register_test(testname_t, Test*, Test* = 0);

	/**Register a new suite with the TestManager.
	 * \param a string of the suite's name
	 * \param a pointer to the suite. Follows the same rules as
	 * those for `register_test()`.*/
	void register_suite(testsuitename_t, TestSuite*);

	/** Run a test or suite intelligently.
	 * \param the name of the item to run
	 * \param the number of times to repeat the test (ignored on suites)
	 * \return true if the item ran successfully, else false */
	bool run(testname_t, unsigned int = 1);

	/** Run all tests.
	 * \return true if the items ran successfully, else false */
	bool runall();

	/**Run a test by name.
	 * \param the name of the test to run
	 * \param the number of times to repeat the test
	 * \return true if the test ran successfully, else false */
	bool run_test(testname_t, unsigned int = 1);

	/**Run a suite by name.
	 * \param the name of the suite to run
	 * \return true if the suite ran successfully, else false */
	bool run_suite(testsuitename_t);

	/**Measure the approximate number of CPU cycles that a particular
	 * test takes to run.
	 *
	 * WARNING: This tool is not a replacement for a proper code
	 * profiler. See the documentation for more information.
	 * \param the name of the test to benchmark
	 * \param the number of times to run the test
	 * (10 - 10,000, default 100). The more repetitions we have,
	 * the more accurate the benchmark will be - but bear execution
	 * time in mind, as some tests can take a while.
	 * \param whether to print all of the output, or just the summaried
	 * verdict.
	 * \param whether a dead heat is a success result
	 * \return true if the first test wins, else false.
	 */
	bool run_benchmark(testname_t,
					   unsigned int = 100,
					   bool = true,
					   bool = true);

	/**Benchmark and compare two tests using a three-pass system.
	 * The three passes - MAMA BEAR, PAPA BEAR, and BABY BEAR -
	 * handle test execution sequence differently to account for
	 * cache warming factors.
	 *
	 * The tests should be designed to perform the exact same logical
	 * task by two different methods.
	 *
	 * WARNING: This tool is not a replacement for a proper code
	 * profiler. See the documentation for more information.
	 * \param the name of test A
	 * \param the name of test B
	 * \param the number of times to run each test per pass
	 * (10 - 10,000, default 100). The more repetitions we have,
	 * the more accurate the benchmark will be - but bear execution
	 * time in mind, as some tests can take a while.
	 * In short, we have 100 repetitions each * 2 tests * 3 passes,
	 * or 100 => 600 total repetitions.
	 * \param whether to print all of the output, or just the summaried
	 * verdict.
	 * \param whether a dead heat is a success result
	 * \return true if the first test wins, else false.
	 */
	bool run_compare(
		testname_t, testname_t, unsigned int = 100, bool = true, bool = true);

	// INTERACTIVE SYSTEM

	/**Interactively (confirm before start) load all suites.
	 * \param the suite name to load (or leave empty to load all) */
	void i_load_suite(testsuitename_t = "");

	void i_run(testname_t, unsigned int = 1);

	/**Interactively (confirm before start) run a test by name.
	 * \param the name of the test to run*/
	void i_run_test(testname_t, unsigned int = 1);

	/**Interactively (confirm before start) run a suite by name.
	 * \param the name of the suite to run*/
	void i_run_suite(testsuitename_t);

	/**Interactively run a benchmark by name.
	 *
	 * \param the name of the test to benchmark
	 * \param the number of times to run the test */
	void i_run_benchmark(testname_t, unsigned int = 100);

	/**Interactively (confirm before start) run a comparative
	 * benchmark by test name.
	 * \param the name of test A
	 * \param the name of test B
	 * \param the number of times to run each test per pass
	 * \param whether to print all of the output, or just the summaried
	 * verdict */
	void i_run_compare(testname_t, testname_t, unsigned int = 100, bool = true);

	/**We don't need anything in the destructor, as the smart pointers
	 * handle deletion automatically.*/
	~TestManager() {}

protected:
	/** Confirm y/N from the user. Used by the interactive functions.
	 * \param the default option (true=yes, false=no)
	 * \return true if confirmed, else false */
	bool i_confirm(bool = false);

	/**Validate that the test (by name) is registered with Golidlocks.
	 * this is critical in preventing segfaults from accessing
	 * invalid tests.
	 *
	 * \param the name of the test to check for
	 * \param whether to display an error message if there is no match
	 * \param whether we're searching for a suite (default, test)
	 * \return true if the test exists, else false
	 */
	bool validate(testname_t,
				  bool = false,
				  GolidlocksItemType = GolidlocksItemType::test);

	/**Benchmark and compare two tests using a three-pass system.
	 * The three passes - MAMA BEAR, PAPA BEAR, and BABY BEAR -
	 * handle test execution sequence differently to account for
	 * cache warming factors.
	 *
	 * The tests should be designed to perform the exact same logical
	 * task by two different methods.
	 *
	 * WARNING: This tool is not a replacement for a proper code
	 * profiler. See the documentation for more information.
	 * \param the name of test A
	 * \param the name of test B
	 * \param the number of times to run each test per pass
	 * (10 - 10,000, default 100). The more repetitions we have,
	 * the more accurate the benchmark will be - but bear execution
	 * time in mind, as some tests can take a while.
	 * In short, we have 100 repetitions each * 2 tests * 3 passes,
	 * or 100 => 600 total repetitions.
	 * \param whether to print all of the output, or just the summaried
	 * verdict.
	 * \param whether a dead heat is a success result
	 * \return true if the first test wins, else false
	 */
	bool run_compare(
		Test*, Test*, unsigned int = 100, bool = true, bool = true);

	/**The BenchmarkResult struct stores all of the statistical data
	 * from a single test benchmark. Having this struct makes our
	 * code more efficient, and allows us to write standard
	 * functions for calculating results and comparisons, and
	 * outputting everything.*/
	struct BenchmarkResult {
		/// The accumulated count.
		// cppcheck-suppress unusedStructMember
		uint64_t acc = 0;

		/// The adjusted accumulated count.
		// cppcheck-suppress unusedStructMember
		uint64_t acc_adj = 0;

		/// The number of repetitions (size).
		// cppcheck-suppress unusedStructMember
		uint64_t repeat = 0;

		/// The average (mean).
		// cppcheck-suppress unusedStructMember
		uint64_t mean = 0;

		/// The adjusted average (mean).
		// cppcheck-suppress unusedStructMember
		uint64_t mean_adj = 0;

		/// The median value.
		// cppcheck-suppress unusedStructMember
		uint64_t median = 0;

		/// The lower quartile value.
		// cppcheck-suppress unusedStructMember
		uint64_t q1 = 0;

		/// The upper quartile value.
		// cppcheck-suppress unusedStructMember
		uint64_t q3 = 0;

		/// The range.
		// cppcheck-suppress unusedStructMember
		uint64_t range = 0;

		/// The adjusted range.
		// cppcheck-suppress unusedStructMember
		uint64_t range_adj = 0;

		/// The minimum value.
		// cppcheck-suppress unusedStructMember
		uint64_t min_val = 0;

		/// The minimum non-outlier value.
		// cppcheck-suppress unusedStructMember
		uint64_t min_adj_val = 0;

		/// The maximum value.
		// cppcheck-suppress unusedStructMember
		uint64_t max_val = 0;

		/// The maximum non-outlier value.
		// cppcheck-suppress unusedStructMember
		uint64_t max_adj_val = 0;

		/// Lower inner fence value.
		// cppcheck-suppress unusedStructMember
		uint64_t lif = 0;

		/// Lower outer fence value
		// cppcheck-suppress unusedStructMember
		uint64_t lof = 0;

		/// Upper inner fence value.
		// cppcheck-suppress unusedStructMember
		uint64_t uif = 0;

		/// Upper outer fence value.
		// cppcheck-suppress unusedStructMember
		uint64_t uof = 0;

		/// The number of low minor outliers
		// cppcheck-suppress unusedStructMember
		uint64_t low_out_minor = 0;

		/// The number of low major outliers
		// cppcheck-suppress unusedStructMember
		uint64_t low_out_major = 0;

		/// The number of upper minor outliers
		// cppcheck-suppress unusedStructMember
		uint64_t upp_out_minor = 0;

		/// The number of upper major outliers
		// cppcheck-suppress unusedStructMember
		uint64_t upp_out_major = 0;

		/// The standard deviance
		// cppcheck-suppress unusedStructMember
		double std_dev = 0;

		/// The adjusted standard deviance
		// cppcheck-suppress unusedStructMember
		double std_dev_adj = 0;

		/// The relative standard deviation (coefficient of variation)
		// cppcheck-suppress unusedStructMember
		uint8_t rsd = 0;

		/// The adjusted relative standard deviation
		// cppcheck-suppress unusedStructMember
		uint8_t rsd_adj = 0;
	};

	/**Convert a raw array of clock measurements into a complete
	 * benchmark result. This does all of our statistical computations.
	 * \param the BenchmarkResult instance to write to
	 * \param the array of CPU cycle measurements, stored as
	 * 64-bit integers
	 * \param the size of the array (number of measurements)*/
	void resultFromArray(BenchmarkResult&, uint64_t arr[], int);

	/**Print the data from a BenchmarkResult. This uses the
	 * standard IOSqueak channel. Output formatting is retained until the end
	 * of the function, so you can modify formatting by preceding this
	 * function call with `channel << [SOME FORMATTING TAGS] << IOCtrl::send;`
	 * \param the BenchmarkResult to output from */
	void printResult(BenchmarkResult&);

	/** Calculate the final verdict based on adjusted results.
	 * \param the BenchmarkResult from test A
	 * \param the BenchmarkResult from test B
	 * \return 0 if even, 1 if A wins, 2 if B wins.
	 */
	uint8_t calculateVerdict(BenchmarkResult&, BenchmarkResult&);

	/**Compare and print two BenchmarkResults. This calculates which
	 * result was faster, based on the adjusted statistics (outliers
	 * removed), and factoring uncertainty into the verdict.
	 * Output formatting is retained until the end
	 * of the function, so you can modify formatting by preceding this
	 * function call with `channel << [SOME FORMATTING TAGS] << IOCtrl::send;`
	 * \param the BenchmarkResult from test A
	 * \param the BenchmarkResult from test B
	 * \param the name of test A (optional)
	 * \param the name of test B (optional)*/
	void printVerdict(BenchmarkResult&, BenchmarkResult&, Test*, Test*);

	/** Calibrate the CPU clocking functionality by serializing the
	 * assembly instruction cache, per Intel's documentation on RDTSC.*/
	void calibrate();

	/** Measure the number of CPU cycles it takes to execute a test
	 * (or just the measurement itself, to get a baseline measurement).
	 * To increase average accuracy, this does not serialize execution.
	 * We are assuming that an out-of-order measurement will get removed
	 * as an outlier in a set of measurements.
	 * \param the raw pointer to the test to run. If omitted, no test
	 * will be executed, allowing us to measure the execution time of
	 * the measurement instructions themselves.
	 * \return the CPU cycle measurement as a 64-bit unsigned integer*/
	uint64_t clock(Test* = 0);

	/** Print the banner for the benchmarker functions using IOSqueak.*/
	void benchmark_banner();

	/** Stores all of the test pointers for access-by-name-string. */
	std::map<testname_t, testptr_t> tests;

	/** Stores all of the testsuite pointers for access-by-name-string. */
	std::map<testsuitename_t, testsuiteptr_t> suites;

	/** Stores all of the comparative test pointers for
	 * access-by-name-string. */
	std::map<testname_t, testptr_t> comparatives;

	/* We are using std::map intentionally above. Dynamic allocation is
	 * more appropriate in this situation, especially since test
	 * registration should be on-demand and front-loaded (all at once).*/
};

#endif  // GOLDILOCKS_MANAGER_HPP
