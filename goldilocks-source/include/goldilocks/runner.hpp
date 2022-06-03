/** Runner [Goldilocks]
 * Version: 2.0
 *
 * A Runner executes the actual tests.
 *
 * Author(s): Wilfrantz Dede, Jason C. McDonald
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

#ifndef GOLDILOCKS_RUNNER_HPP
#define GOLDILOCKS_RUNNER_HPP

#include <cstdint>
#include <variant>

#include "goldilocks/benchmark_results.hpp"
#include "goldilocks/clock.hpp"
#include "goldilocks/suite.hpp"
#include "goldilocks/test.hpp"
#include "goldilocks/types.hpp"

// Empty runner to allow for specialization
template<typename T> class Runner;

// Runner for Tests
template<> class Runner<Test>
{
protected:
	Test* test;
	Test* comparative;
	uint16_t iterations;

public:
	/* Ctor, stores test information
	 * \param test The test to run
	 * \param comparative The test to compare to
	 * \param iterations The number of times to repeat the test
	 */
	Runner(Test* test, Test* comparative, uint16_t iterations = 1)
	: test(test), comparative(comparative), iterations(iterations)
	{
	}

	// TODO: Add Reports
	/* Runs the test
	 * \return bool True if the test is succesful, false if not
	 */
	virtual bool run()
	{
		// run pre() from test. If it fails, call prefail()
		if (!this->test->pre()) {
			this->test->prefail();
			return false;
		}

		for (uint16_t i = 0; i < this->iterations; ++i) {
			// run janitor() from test. If fails, call postmortem()
			if (!this->test->janitor()) {
				this->test->postmortem();
				return false;
			}

			// run run() from test. If fails && test exits on failure, run
			// posrtmortem()
			if (!this->test->run()) {
				return false;
			}  // TODO: If exit on fail.
		}

		this->test->post();
		return true;
	}

	~Runner() = default;
};

class BenchmarkRunner final : public Runner<Test>
{
protected:
	BenchmarkResult results;

public:
	/* Ctor for the BenchmarkRunner
	 * \param test The test to run
	 * \param comparative The test to compare it to
	 * \param iterations The number of times to run the test
	 */
	BenchmarkRunner(Test* test, Test* comparative, uint16_t iterations = 1)
	: Runner(test, comparative, iterations), results()
	{
	}

	/* Runs the test
	 * \return bool True if the test is succesful, false if not
	 */
	bool run() override
	{
		// Initialize test
		if (!this->test->pre()) {
			this->test->prefail();
			return false;
		}
		// Validate that test runs.
		if (!this->test->run_optimized()) {
			this->test->postmortem();
			return false;
		}
		// Initialize comparative
		if (!this->comparative->pre()) {
			this->comparative->prefail();
			this->test->post();
			return false;
		}

		if (!this->comparative->run_optimized()) {
			this->comparative->postmortem();
			this->test->post();
			return false;
		}

		// Actual benchmarking
		for (uint16_t i = 0; i < this->iterations; ++i) {
			if (!this->test->janitor()) {
				this->test->postmortem();
				this->comparative->post();
				return false;
			}

			if (!this->comparative->janitor()) {
				this->comparative->postmortem();
				this->test->post();
				return false;
			}

			// clock() calls test->run_optimized() and
			// comparative->run_optimized()
			this->results.add_measurement(clock(this->test),
										  clock(this->comparative));
		}

		this->test->post();
		this->comparative->post();
		return true;
	}

	~BenchmarkRunner() = default;
};

/* Runs the given item, to be used by the runner
 * \param arg A Runnable, either a suite or a test
 * \return bool True if the test is succesful, false if not
 */
template<typename T> bool run_item(T arg)
{
	using Type = std::decay_t<decltype(arg)>;
	if constexpr (std::is_same_v<Type, TestSuite*>) {
		for (auto items : arg->runnables) {
			run_item(items.second);
		}
	} else if constexpr (std::is_same_v<Type, Test*>) {
		return arg->run();
	}
}

template<> class Runner<TestSuite>
{
protected:
	TestSuite* suite;
	uint16_t iterations;

public:
	/* Ctor To run either a suite or a test
	 * \param suite The suite to run
	 * \param iterations The number of time to run the suite
	 */
	explicit Runner(TestSuite* suite, uint16_t iterations = 1)
	: suite(suite), iterations(iterations){};

	/* Runs the suite a given amount of time
	 */
	void run()
	{
		for (uint16_t i = 0; i <= iterations; i++) {
			for (auto suite_pair : suite->runnables) {
				// TODO: Do something with the result (i.e, generate a report)
				bool res = std::visit([](auto arg) { return run_item(arg); },
									  suite_pair.second);
			}
		}
	}
};
#endif  // GOLDILOCKS_RUNNER_HPP
