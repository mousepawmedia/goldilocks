/** Benchmark Runner [Goldilocks]
 * Version: 2.0
 *
 * Runner class for Benchmarking.
 *
 * Author(s): Wilfrantz DEDE, Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
* Copyright (c) 2021 MousePaw Media.
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
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
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

#ifndef BENCHMARKRUNNER_HPP
#define BENCHMARKRUNNER_HPP

#include <cstdint>

#include "goldilocks/benchmark_results.hpp"
#include "goldilocks/runner.hpp"
#include "goldilocks/test.hpp"

class BenchmarkRunner final : public Runner
{
protected:
	BenchmarkResult results;

public:
	BenchmarkRunner(Test* test, Test* comparative, uint16_t iterations = 1)
	: Runner(test, comparative, iterations), results()
	{
	}

	void run() override;

	~BenchmarkRunner() = default;
};

#endif  // BENCHMARKRUNNER_HPP

