/** Types and Enums [Goldilocks]
 * Version: 2.0
 *
 * Author(s): Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2019 MousePaw Media.
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

#ifndef GOLDILOCKS_TYPES_HPP
#define GOLDILOCKS_TYPES_HPP

#include <memory>
#include <string>

/// Represents that no exception is thrown.
class Nothing
{
public:
	Nothing() {}
};

// NOTE: We don't need to represent a void return; it never needs to be tested!

std::ostream& operator<<(std::ostream& out, const Nothing&)
{
	out << "[Nothing]";
	return out;
}

/// The type we use for storing test names.
typedef std::string itemname_t;
/// The type we use for storing documentation strings. Alias of itemname_t.
typedef itemname_t testdoc_t;

class Test;
class TestSuite;

/// The type we use for unique pointers of tests.
typedef std::unique_ptr<Test> testptr_t;
/// The type we use for unique pointers of suites.
typedef std::unique_ptr<TestSuite> testsuiteptr_t;

/// Types of runnable objects in Goldilocks
enum class Item { Test, Comparative, Suite };

/// Run modes for runnable objects.
enum class Mode { Test, Benchmark, Verify };

/// Status of runnable objects.
enum class Status { OK, Prefail, Warn, Fail, Postfail, Confused };

#endif  // GOLDILOCKS_TYPES_HPP
