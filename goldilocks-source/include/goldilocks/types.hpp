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

#include <string>
#include <tuple>

class Nothing
{
public:
	Nothing() {}
};

/* The type we use for storing test names. */
typedef std::string testname_t;
/* The type we use for storing suite names. Alias of testname_t.*/
typedef testname_t suitename_t;
/* The type we use for storing documentation strings. Alias of testname_t.*/
typedef testname_t testdoc_t;

enum class Item { Test, Comparative, Suite };

enum class Mode { Test, Benchmark, Verify };

enum class Status { OK, Prefail, Warn, Fail, Postfail, Confused };

/* 
 * A comment after the enum value indicates
 * an outcome method has been made in outcomes.hpp,
 * if a That type is being added, please create the function
 * and comment.
 */
enum class That {
	IsTrue,          // check_true()
	IsFalse,         // check_false()
	IsEqual,         // check_eq()
	IsNotEqual,      // check_not_eq()
	IsLess,          // check_less ()
	IsLessEqual,     // check_less_eq()
	IsGreater,       // check_greater()
	IsGreaterEqual,  // check_greater_eq()
	PtrIsNull,       // check_nullptr()
	PtrIsNotNull,    // check_not_nullptr()
	PtrIsEqual,      // check_ptr_equal()
	PtrIsNotEqual,   // check_ptr_not_equal()
	FuncReturns,     // check_func_returns()
	FuncThrows       // check_func_throws()
};

enum class Should { Pass, Fail };

// TODO: Can we ultimately move this to IOSqueak?
std::string stringify(const That& op)
{
	switch (op) {
		case That::IsTrue:
			return "== true";
		case That::IsFalse:
			return "== false";
		case That::IsEqual:
			return "==";
		case That::IsNotEqual:
			return "!=";
		case That::IsLess:
			return "<";
		case That::IsLessEqual:
			return "<=";
		case That::IsGreater:
			return ">";
		case That::IsGreaterEqual:
			return ">=";
		case That::PtrIsNull:
			return "== nullptr";
		case That::PtrIsNotNull:
			return "!= nullptr";
		case That::PtrIsEqual:
			return "==";
		case That::PtrIsNotEqual:
			return "!=";
		case That::FuncReturns:
			return "==>";
		case That::FuncThrows:
			return "~~>";
	}
}

// TODO: Can we ultimately move this to IOSqueak?
std::string stringify(const Should& op)
{
	switch (op) {
		case Should::Pass:
			return "";
		case Should::Fail:
			return "[Should Fail]";
	}
	return "";
}

#endif  // GOLDILOCKS_TYPES_HPP
