/** Report [Goldilocks]
 * Version: 2.0
 *
 * Report class for outputting result of tests.
 *
 * Author(s): Tianlin Fu
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

#ifndef REPORT_HPP
#define REPORT_HPP
#include <cstdint>
#include <cxxabi.h>
#include <string>
#include <vector>

#include "goldilocks/expect/expect.hpp"
#include "goldilocks/expect/outcomes.hpp"
#include "goldilocks/expect/should.hpp"
#include "goldilocks/expect/that.hpp"
#include "goldilocks/test.hpp"
#include "goldilocks/types.hpp"
class Report
{
private:
	std::vector<std::string> reports;

protected:
public:
	// Init report
	Report() {}

	// Add outcome of a single test in the form of a expect object
	template<typename H, Should S = Should::Pass>
	void log(const Expect<H, S>& expect)
	{
		char* realname;
		realname = abi::__cxa_demangle(typeid(H).name(), 0, 0, 0);
		std::string name(realname);
		reports.push_back("\nTesting: " + name + "\n");
		reports.push_back(expect.compose() + "\n");
	}

	// Add measurement from test
	template<typename U> void log(const U& measurements)
	{
		std::string measure = std::to_string(measurements);
		reports.push_back("Test measurement: " + measure + "\n\n");
	}

	// Ensures that the test run is complete
	void lap() { reports.push_back("End of test. \n"); }

	// Prints out the reports for the test
	void print_report()
	{
		int size = reports.size();
		for (int i = 0; i < size; i++) {
			std::cout << reports[i];
		}
	}
	// Destructor
	~Report() { reports.clear(); }
};

#endif  // REPORT_HPP