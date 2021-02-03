/** Types [Goldilocks]
 * Version: 1.3
 *
 * Type aliases and enumerations for Goldilocks.
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

#ifndef GOLDILOCKS_TYPES_HPP
#define GOLDILOCKS_TYPES_HPP

// std::unique_ptr
#include <memory>
// std::vector (we will use this here)
#include <vector>

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

enum class GolidlocksItemType { test, comparative, suite };

#endif // GOLDILOCKS_TYPES_HPP
