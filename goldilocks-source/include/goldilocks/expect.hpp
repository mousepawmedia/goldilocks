/** Assert [Goldilocks]
 * Version: 2.0
 *
 * Assert functionality for Goldilocks.
 *
 * Author(s): Jason C. McDonald
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

#ifndef GOLDILOCKS_EXPECT_HPP
#define GOLDILOCKS_EXPECT_HPP

#include <iostream>

#include "goldilocks/outcomes.hpp"
#include "goldilocks/types.hpp"
#include "iosqueak/stringify.hpp"

/*
 * TODO: Can we repeat ourselves less? Any way to make it so, for
 * instance, we don't have to write so many expects? Why can't
 * Expect<That::IsTrue> just be something like
 * That::IsTrue(check_true, check_true_ptr).
 */

/* TODO: Specialize Expect instead of overload the constructor!
 * There is no need to recreate the entire base class, just to provide diff
 * functions. This will create COMPILER errors instead of RUNTIME errors when
 * the constructor is done wrong!
 */

template<That H, Should S>
class _BaseExpect
{
protected:
	bool success;
	testdoc_t message;

	template<typename T>
	_BaseExpect(const T& operand, const Outcome& outcome)
	: success(outcome.success(S)),
	  message(compose(operand, outcome.success(S)))
	{
	}

	template<typename T, typename U>
	_BaseExpect(const T& lhs, const U& rhs, const Outcome& outcome)
	: success(outcome.success(S)),
	  message(compose(lhs, rhs, outcome.success(S)))
	{
	}

	// TODO: Handle FuncReturns and FuncThrows

	bool status() const { return success; }

	template<typename T>
	static std::string compose(const T& op, const bool& success)
	{
		std::string msg = std::string();
		msg.reserve(128);
		msg.append(stringify(success, IOFormatBool::test))
			.append(": ")
			.append(stringify(op))
			.append(" ")
			.append(stringify(H))
			.append(" ")
			.append(stringify(S))
			.shrink_to_fit();
		return msg;
	}

	template<typename T, typename U>
	static std::string compose(const T& lhs, const U& rhs, const bool& success)
	{
		std::string msg = std::string();
		msg.reserve(128);
		msg.append(stringify(success, IOFormatBool::test))
			.append(": ")
			.append(stringify(lhs))
			.append(" ")
			.append(stringify(H))
			.append(" ")
			.append(stringify(rhs))
			.append(" ")
			.append(stringify(S))
			.shrink_to_fit();
		return msg;
	}

	// TODO: Make this class abstract maybe?



public:
	template<That OH, Should OS>
	friend std::ostream& operator<<(std::ostream&, const _BaseExpect<OH, OS>&);
};

template<That H, Should S = Should::Pass>
class Expect : public _BaseExpect<H, S>
{
};

template<Should S>
class Expect<That::IsTrue, S> : public _BaseExpect<That::IsTrue, S>
{
public:
	template<typename T>
	explicit Expect(const T& op)
	: _BaseExpect<That::IsTrue, S>(op, check_true(op))
	{
	}

	template<typename T>
	explicit Expect(const T* op)
	: _BaseExpect<That::IsTrue, S>(op, check_true_ptr(op))
	{
	}
};

template<Should S>
class Expect<That::IsFalse, S> : public _BaseExpect<That::IsFalse, S>
{
public:
	template<typename T>
	explicit Expect(const T& op)
	: _BaseExpect<That::IsFalse, S>(op, check_false(op))
	{
	}

	template<typename T>
	explicit Expect(const T* op)
	: _BaseExpect<That::IsFalse, S>(op, check_false_ptr(op))
	{
	}
};

template<Should S>
class Expect<That::IsEqual, S> : public _BaseExpect<That::IsEqual, S>
{
public:
	template<typename T, typename U>
	Expect(const T& lhs, const U& rhs)
	: _BaseExpect<That::IsEqual, S>(lhs, rhs, check_eq(lhs, rhs))
	{
	}

	template<typename T, typename U>
	Expect(const T* lhs, const U* rhs)
	: _BaseExpect<That::IsEqual, S>(lhs, rhs, check_eq_ptr(lhs, rhs))
	{
	}
};

template<Should S>
class Expect<That::IsNotEqual, S> : public _BaseExpect<That::IsNotEqual, S>
{
public:
	template<typename T, typename U>
	Expect(const T& lhs, const U& rhs)
	: _BaseExpect<That::IsNotEqual, S>(lhs, rhs, check_not_eq(lhs, rhs))
	{
	}

	template<typename T, typename U>
	Expect(const T* lhs, const U* rhs)
	: _BaseExpect<That::IsNotEqual, S>(lhs, rhs, check_not_eq_ptr(lhs, rhs))
	{
	}
};

template<Should S>
class Expect<That::IsLess, S> : public _BaseExpect<That::IsLess, S>
{
public:
	template<typename T, typename U>
	Expect(const T& lhs, const U& rhs)
	: _BaseExpect<That::IsLess, S>(lhs, rhs, check_less(lhs, rhs))
	{
	}

	template<typename T, typename U>
	Expect(const T* lhs, const U* rhs)
	: _BaseExpect<That::IsLess, S>(lhs, rhs, check_less_ptr(lhs, rhs))
	{
	}
};

template<Should S>
class Expect<That::IsLessEqual, S> : public _BaseExpect<That::IsLessEqual, S>
{
public:
	template<typename T, typename U>
	Expect(const T& lhs, const U& rhs)
	: _BaseExpect<That::IsLessEqual, S>(lhs, rhs, check_less_eq(lhs, rhs))
	{
	}

	template<typename T, typename U>
	Expect(const T* lhs, const U* rhs)
	: _BaseExpect<That::IsLessEqual, S>(lhs, rhs, check_less_eq_ptr(lhs, rhs))
	{
	}
};

template<Should S>
class Expect<That::IsGreater, S> : public _BaseExpect<That::IsGreater, S>
{
public:
	template<typename T, typename U>
	Expect(const T& lhs, const U& rhs)
	: _BaseExpect<That::IsGreater, S>(lhs, rhs, check_greater(lhs, rhs))
	{
	}

	template<typename T, typename U>
	Expect(const T* lhs, const U* rhs)
	: _BaseExpect<That::IsGreater, S>(lhs, rhs, check_greater_ptr(lhs, rhs))
	{
	}
};

template<Should S>
class Expect<That::IsGreaterEqual, S> : public _BaseExpect<That::IsGreaterEqual, S>
{
public:
	template<typename T, typename U>
	Expect(const T& lhs, const U& rhs)
	: _BaseExpect<That::IsGreaterEqual, S>(lhs, rhs, check_greater_eq(lhs, rhs))
	{
	}

	template<typename T, typename U>
	Expect(const T* lhs, const U* rhs)
	: _BaseExpect<That::IsGreaterEqual, S>(lhs,
										   rhs,
										   check_greater_eq_ptr(lhs, rhs))
	{
	}
};

template<Should S>
class Expect<That::PtrIsNull, S> : public _BaseExpect<That::PtrIsNull, S>
{
public:
	template<typename T>
	explicit Expect(const T* op)
	: _BaseExpect<That::PtrIsNull, S>(op, check_nullptr(op))
	{
	}
};

template<Should S>
class Expect<That::PtrIsNotNull, S> : public _BaseExpect<That::PtrIsNotNull, S>
{
public:
	template<typename T>
	explicit Expect(const T* op)
	: _BaseExpect<That::PtrIsNotNull, S>(op, check_not_nullptr(op))
	{
	}
};

template<Should S>
class Expect<That::PtrIsEqual, S> : public _BaseExpect<That::PtrIsEqual, S>
{
public:
	template<typename T, typename U>
	explicit Expect(const T* lhs, const U* rhs)
	: _BaseExpect<That::PtrIsEqual, S>(lhs, rhs, check_ptr_equal(lhs, rhs))
	{
	}
};

template<Should S>
class Expect<That::PtrIsNotEqual, S> : public _BaseExpect<That::PtrIsNotEqual, S>
{
public:
	template<typename T, typename U>
	explicit Expect(const T* lhs, const U* rhs)
	: _BaseExpect<That::PtrIsNotEqual, S>(lhs,
										  rhs,
										  check_ptr_not_equal(lhs, rhs))
	{
	}
};

// TODO: _BaseExpectFunc for the following two
/*
template<Should S>
class Expect<That::FuncReturns, S> : public _BaseExpect<That::FuncReturns, S>
{
public:
	template<typename T, typename U, typename... Args>
	Expect(const T& target, const char* name_hint, const U func, Args... args)
	: _BaseExpect<That::FuncReturns, S>(target, func, check_func_returns(target,
func, args)))
	{
		// TODO: Improve to show ACTUAL and EXPECTED returns.
		message = stringify(name_hint, func, args...) + " " +
				  stringify(That::FuncReturns) + " " + stringify(target) + " " +
				  stringify(S);
	}
};

template<Should S>
class Expect<That::FuncThrows, S> : public _BaseExpect<That::FuncThrows, S>
{
public:
	template<typename T, typename U, typename... Args>
	Expect(const T& target, const char* name_hint, const U func, Args... args)
	: _BaseExpect<That::FuncReturns, S>(target, func, check_func_returns(target,
func, args)))
	{
		try {
			func(args...);
		} catch (const T& e) {
			success = true;
		} catch (const std::exception& e) {
			// If any other exception is caught, still fail.
			success = false;
		}

		// TODO: Improve to show ACTUAL and EXPECTED returns.
		message = stringify(name_hint, func, args...) + " " +
				  stringify(That::FuncThrows) + " " + stringify(target) + " " +
				  stringify(S);
	}
};

*/

template<That H, Should S = Should::Pass>
std::ostream& operator<<(std::ostream& out, const _BaseExpect<H, S>& expect)
{
	// TODO: Temporary only!
	out << expect.message << " ";
	return out;
}

#endif
