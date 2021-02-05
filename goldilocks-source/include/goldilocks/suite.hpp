/** Suite [Goldilocks]
 * Version: 1.3
 *
 * Base class for Goldilocks suites, which are collections of tests.
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

#ifndef GOLDILOCKS_SUITE_HPP
#define GOLDILOCKS_SUITE_HPP

// std::vector
#include <vector>

#include "goldilocks/test.hpp"
#include "goldilocks/types.hpp"
#include "iosqueak/channel.hpp"

/**A TestSuite is responsible for registering a batch of Tests with
 * Goldilocks Test Manager on demand. This is primarily useful if an
 * interactive test console is implemented, as we can load batches of
 * tests when, and only when, we need them. A TestSuite should also
 * be able to report what tests it contains, for user reference while
 * using the interactive test console.
 */
class TestSuite
{
	friend class TestManager;

public:
	TestSuite() : testmanager(0), loaded(false) {}

	/** Load all of the tests into the testsystem and suite.
	 */
	virtual void load_tests() = 0;

	virtual testdoc_t get_title() = 0;

	/** Run all of the tests in the suite.
	 * \return true if all the tests run, else false
	 */
	bool run_tests();  // HACK: Definition necessarily in manager.cpp.

	/** Check if the testsuite was already loaded, or mark it as loaded.
	 * Used directly by Goldilocks.
	 * \param whether to mark the suite as loaded
	 * \return true if loaded, else false */
	bool is_loaded(bool mark = false)
	{
		if (mark) {
			loaded = true;
		}
		return loaded;
	}

	virtual ~TestSuite() {}

protected:
	/** Register a test with both the suite and the testmanager.
	 * This is here purely for end-developer convenience when creating
	 * a derived TestSuite.
	 * \param the name of the test
	 * \param the pointer to the test (declare "new" for this argument)
	 * \param whether to run the test on suite run (default true)
	 */
	void register_test(
		testname_t test_name,
		Test* test,
		bool will_run = true,
		Test* compare = 0);  // HACK: Definition necessarily in manager.cpp.

	/// The list of tests to load. Mainly here for reference right now.
	std::vector<testname_t> tests_load;

	/** The list of tests to run. This should always be a subset
	 * of tests_load. */
	std::vector<testname_t> tests_run;

	/** We store the pointer to the testmanager we're using in
	 * this instance for ease of test registration. */
	TestManager* testmanager;

	/** Tracks whether we've already loaded this suite, to prevent
	 * loading multiple times into one testmanager. */
	bool loaded;

private:
	/** Completes registration of a suite with the testmanager.
	 * This is only accessible by TestManager: it is even hidden
	 * from the derived classes and end-developer (intentionally).
	 * \param the pointer to the TestManager controlling the suite.
	 */
	void backregister(TestManager* tm) { testmanager = tm; }
};

#endif  // GOLDILOCKS_SUITE_HPP
