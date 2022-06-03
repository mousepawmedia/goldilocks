/** Benchmark Results [Goldilocks]
 * Version: 2.0
 *
 * Class for storing results of benchmarking (clocking)
 *
 * Author(s): Wilfrantz DEDE, Manuel Mateo, Jason C. McDonald
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

#ifndef BENCHMARKRESULTS_HPP
#define BENCHMARKRESULTS_HPP

#include <cstdint>
#include <vector>

enum class BenchmarkVerdict {
	/// No result yet
	none,
	/// Tests are roughly equal
	draw,
	/// Primary (Test A) wins
	win,
	/// Comparative (Test B) wins
	loss,
	/// RSD too high, result is unreliable.
	questionable
};

class BenchmarkResult
{
private:
	BenchmarkVerdict verdict;

	/// Vector to store the raw measurements
	std::vector<uint64_t> results;

	/// The accumulated count.
	uint64_t acc = 0;

	/// The adjusted accumulated count.
	uint64_t acc_adj = 0;

	/// The number of repetitions (size).
	uint64_t repeat = 0;

	/// The average (mean).
	uint64_t mean = 0;

	/// The adjusted average (mean).
	uint64_t mean_adj = 0;

	/// The median value.
	uint64_t median = 0;

	/// The lower quartile value.
	uint64_t q1 = 0;

	/// The upper quartile value.
	uint64_t q3 = 0;

	/// The range.
	uint64_t range = 0;

	/// The adjusted range.
	uint64_t range_adj = 0;

	/// The minimum value.
	uint64_t min_val = 0;

	/// The minimum non-outlier value.
	uint64_t min_adj_val = 0;

	/// The maximum value.
	uint64_t max_val = 0;

	/// The maximum non-outlier value.
	uint64_t max_adj_val = 0;

	/// Lower inner fence value.
	uint64_t lif = 0;

	/// Lower outer fence value
	uint64_t lof = 0;

	/// Upper inner fence value.
	uint64_t uif = 0;

	/// Upper outer fence value.
	uint64_t uof = 0;

	/// The number of low minor outliers
	uint64_t low_out_minor = 0;

	/// The number of low major outliers
	uint64_t low_out_major = 0;

	/// The number of upper minor outliers
	uint64_t upp_out_minor = 0;

	/// The number of upper major outliers
	uint64_t upp_out_major = 0;

	/// The standard deviance
	double std_dev = 0;

	/// The adjusted standard deviance
	double std_dev_adj = 0;

	/// The relative standard deviation (coefficient of variation)
	uint8_t rsd = 0;

	/// The adjusted relative standard deviation
	uint8_t rsd_adj = 0;

public:
	BenchmarkResult() : verdict(BenchmarkVerdict::none) {}

	/**Convert a raw array of clock measurements into a complete
	 * benchmark result. This does all of our statistical computations.
	 * \param the BenchmarkResult instance of result A (1)
	 * \param the BenchmarkResult instance of result B (2)
	 * Calculate the final verdict based on adjusted results.
	 */
	void finalize(const BenchmarkResult&, const BenchmarkResult&);

	/*Adds in two numbers, one for each result A or B
	 * \param the clock measurement for result A
	 * \param the clock measurement for result B
	 */
	inline void add_measurement(uint64_t measurement_a, uint64_t measurement_b)
	{
		results.push_back(measurement_a);
		results.push_back(measurement_b);
	}

	~BenchmarkResult();
};

#endif  // BENCHMARKRESULTS_HPP
