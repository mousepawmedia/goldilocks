/** Runner [Goldilocks]
 * Version: 2.0
 *
 * A Runner executes the actual tests.
 *
 * Author(s): Wilfrantz Dede, Jason C. McDonald
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

#ifndef GOLDILOCKS_RUNNER_HPP
#define GOLDILOCKS_RUNNER_HPP

#include <cstdint>  

#include "goldilocks/test.hpp"
#include "goldilocks/types.hpp"

class Runner {
protected:
	Test* test;
	uint16_t iterations;

public:
	// cppcheck-suppress noExplicitConstructor
	Runner(Test* test, uint16_t iterations = 1)
	: test(test), iterations(iterations)
	{}

	void run() {
		// run pre() from test. If it fails, call prefail()
		if(!this->test->pre()) {
			this->test->prefail();
			return; 
		}

		for (uint16_t i = 0; i < this->iterations; ++i) {
			// run janitor() from test. If fails, call postmortem()
			if (!this->test->janitor()) {
				this->test->postmortem();
				return;
			}

			// run run() from test. If fails && test exits on failure, run
			// posrtmortem()
			if (!this->test->run()) {return;}  // TODO: If exit on fail.
		}

		this->test->post();
	}

	~Runner() = default;
};

#endif  // GOLDILOCKS_RUNNER_HPP


