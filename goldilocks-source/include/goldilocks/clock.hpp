/** Clock [Goldilocks]
 * Version: 2.0
 *
 * Raw clocking functions for benchmarking.
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

#include <cstdint>

#include "goldilocks/test.hpp"

// MACRO IF we are using a GCC-style compiler.
// NOTE: We're assuming Intel/AMD. What about PowerPC and ARM?
#if defined __GNUC__ || __MINGW32__ || __MINGW64__

// If we're on a 64-bit system...
#if defined __LP64__

void calibrate()
{
	/* According to Intel's documentation, we must "warms up"
	 * the instruction cache that we'll be using for measurement,
	 * to improve accuracy.*/

	uint32_t low, high;

	/* Run the following commands three times to warm up the
	instructions cache.*/
	for (uint16_t i = 0; i < 3; i++) {
		asm volatile("cpuid;"
					 "rdtsc;"
					 "mov %%edx, %0;"
					 "mov %%eax, %1;"
					 : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");

		asm volatile("rdtsc;"
					 "mov %%edx, %0;"
					 "mov %%eax, %1;"
					 : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");
	}
}

inline uint64_t clock(Test* test = 0)
{
	uint32_t low, high;
	uint64_t cyc1, cyc2;

	// Serialize initially (we won't do it later, though.)
	asm volatile("cpuid;" ::: "%rax", "%rbx", "%rcx", "%rdx");
	// Get the initial timestamp (all 64 bits).
	asm volatile("rdtsc;"
				 "mov %%edx, %0;"
				 "mov %%eax, %1;"
				 : "=r"(high), "=r"(low)::"%rax", "%rdx");
	// Put the high and low halves of the timestamp together and store.
	cyc1 = ((uint64_t)high << 32) | low;

	/* If we have a test, run it. (If there is no test, we will just
	 * wind up measuring the measurement instructions by themselves.*/
	if (test != NULL)
		test->run();

	/* Get the second timestamp. DO NOT SERIALIZE! The CPUID instruction
	 * is too unpredictable in terms of execution size. We can just
	 * assume that an out-of-order execution will yield an outlier
	 * measurement.*/
	asm volatile(/*"cpuid;"*/
				 "rdtsc;"
				 "mov %%edx, %0;"
				 "mov %%eax, %1;"
				 : "=r"(high), "=r"(low)::"%rax", "%rdx");

	// Put the high and low halves of the timestamp together and store.
	cyc2 = ((uint64_t)high << 32) | low;

	// Return the difference between the AFTER and BEFORE timestamps.
	return (cyc2 - cyc1);
}

// If we're on a 32-bit system...
#else

/* See the comments for the 64-bit versions of these functions. The only
 * difference between the 32-bit and 64-bit versions is in which
 * registers we are clobbering. On x86 (32-bit), we clobber %eax-%edx,
 * while on x64, we clobber %rax-%rdx.*/
void calibrate()
{
	uint32_t low, high;

	for (uint16_t i = 0; i < 3; i++) {
		asm volatile("cpuid;"
					 "rdtsc;"
					 "mov %%edx, %0;"
					 "mov %%eax, %1;"
					 : "=r"(high), "=r"(low)::"%eax", "%ebx", "%ecx", "%edx");

		asm volatile("rdtsc;"
					 "mov %%edx, %0;"
					 "mov %%eax, %1;"
					 : "=r"(high), "=r"(low)::"%eax", "%ebx", "%ecx", "%edx");
	}
}

inline uint64_t clock(Test* test = 0)
{
	uint32_t low, high;
	uint64_t cyc1, cyc2;

	asm volatile("cpuid;" ::: "%eax", "%ebx", "%ecx", "%edx");
	asm volatile("rdtsc;"
				 "mov %%edx, %0;"
				 "mov %%eax, %1;"
				 : "=r"(high), "=r"(low)::"%eax", "%edx");
	cyc1 = ((uint64_t)high << 32) | low;
	if (test != NULL)
		test->run_optimized();
	asm volatile(/*"cpuid;"*/
				 "rdtsc;"
				 "mov %%edx, %0;"
				 "mov %%eax, %1;"
				 : "=r"(high), "=r"(low)::"%eax", "%edx");

	cyc2 = ((uint64_t)high << 32) | low;

	return (cyc2 - cyc1);
}
#endif

/* MACRO ELSE if we're not on a GCC, just don't offer the rdtsc functions;
 * offer dummies instead. We will NOT be supporting MSVC under ANY
 * circumstances!*/
#else

// TODO Non-GCC clock functions.
void calibrate() { return; }

inline uint64_t clock(Test* test = 0) { return; }

#endif
