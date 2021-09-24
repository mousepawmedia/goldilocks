/** Suite [Goldilocks]
 * Version: 2.0
 *
 * Base class for Goldilocks suites, which are collections of tests.
 *
 * Author(s): Jason C. McDonald, Manuel E. Mateo
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

#include <vector>
#include <unordered_map>
#include <variant>
#include <stdexcept>

#include "goldilocks/test.hpp"
#include "goldilocks/types.hpp"

/**A TestSuite is responsible for registering a batch of Tests with
 * Goldilocks Test Manager on demand. This is primarily useful if an
 * interactive test console is implemented, as we can load batches of
 * tests when, and only when, we need them. A TestSuite should also
 * be able to report what tests it contains, for user reference while
 * using the interactive test console.
 */

class TestSuite;

using Runnable = std::variant<Test*, TestSuite*>;

class TestSuite 
{
public:
	itemname_t suite_name;
	testdoc_t suite_desc;
	std::unordered_map<itemname_t, Runnable> runnables;
	std::unordered_map<itemname_t, Test*> compares;


	TestSuite(itemname_t suite_name, testdoc_t suite_desc):
	suite_name(suite_name), suite_desc(suite_desc)
	{}

	virtual void load() = 0;

	virtual void register_item(itemname_t item_name, Test* test, Test* compare = nullptr);

	virtual void register_item(itemname_t item_name, TestSuite* suite);
};

#endif  // GOLDILOCKS_SUITE_HPP