/** Outcomes [Goldilocks]
 *
 * Core tests and their outcomes.
 *
 * Author(s): Jacob Frazier, Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
* Copyright (c) 2020 MousePaw Media.
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

#ifndef GOLDILOCKS_OUTCOMES_HPP
#define GOLDILOCKS_OUTCOMES_HPP

#include "goldilocks/types.hpp"

class Outcome
{
protected:
	bool pass;

public:
	explicit Outcome(const bool& pass) : pass(pass) {}

	bool success(const Should& should) const
	{
		switch (should) {
			case Should::Pass:
				return pass;
			case Should::Fail:
				return !pass;
		}
	}
};

template<typename X, typename R>
class ComplexOutcome : public Outcome
{
protected:
	X expected;
	R returned;

public:
	ComplexOutcome<X, R>(const bool& passed,
						 const X& expected,
						 const R& returned)
	: Outcome(passed), expected(expected), returned(returned)
	{
	}
};

template<typename OP>
inline Outcome check_true(const OP& op)
{
	return Outcome(op == true);
}

template<typename OP>
inline Outcome check_true_ptr(const OP* op)
{
	return op == nullptr ? Outcome(false) : check_true(*op);
}

template<typename OP>
inline Outcome check_false(const OP& op)
{
	return Outcome(op == false);
}

template<typename OP>
inline Outcome check_false_ptr(const OP* op)
{
	return op == nullptr ? Outcome(false) : check_false(*op);
}

template<typename LH, typename RH>
inline Outcome check_eq(const LH& lh, const RH& rh)
{
	return Outcome(lh == rh);
}

template<typename LH, typename RH>
inline Outcome check_eq_ptr(const LH& lh, const RH& rh)
{
	return (lh == nullptr || rh == nullptr) ? Outcome(false)
											: check_eq(*lh, *rh);
}

template<typename LH, typename RH>
inline Outcome check_not_eq(const LH& lh, const RH& rh)
{
	return Outcome(lh != rh);
}

template<typename LH, typename RH>
inline Outcome check_not_eq_ptr(const LH& lh, const RH& rh)
{
	return (lh == nullptr || rh == nullptr) ? Outcome(false)
											: check_not_eq(*lh, *rh);
}

template<typename LH, typename RH>
inline Outcome check_less(const LH& lh, const RH& rh)
{
	return Outcome(lh < rh);
}

template<typename LH, typename RH>
inline Outcome check_less_ptr(const LH& lh, const RH& rh)
{
	return (lh == nullptr || rh == nullptr) ? Outcome(false)
											: check_less(*lh, *rh);
}

template<typename LH, typename RH>
inline Outcome check_less_eq(const LH& lh, const RH& rh)
{
	return Outcome(lh <= rh);
}

template<typename LH, typename RH>
inline Outcome check_less_eq_ptr(const LH& lh, const RH& rh)
{
	return (lh == nullptr || rh == nullptr) ? Outcome(false)
											: check_less_eq(*lh, *rh);
}

template<typename LH, typename RH>
inline Outcome check_greater(const LH& lh, const RH& rh)
{
	return Outcome(lh > rh);
}

template<typename LH, typename RH>
inline Outcome check_greater_ptr(const LH& lh, const RH& rh)
{
	return (lh == nullptr || rh == nullptr) ? Outcome(false)
											: check_greater(*lh, *rh);
}

template<typename LH, typename RH>
inline Outcome check_greater_eq(const LH& lh, const RH& rh)
{
	return Outcome(lh >= rh);
}

template<typename LH, typename RH>
inline Outcome check_greater_eq_ptr(const LH& lh, const RH& rh)
{
	return (lh == nullptr || rh == nullptr) ? Outcome(false)
											: check_greater_eq(*lh, *rh);
}

template<typename OP>
inline Outcome check_nullptr(const OP& op)
{
	return Outcome(op == nullptr);
}

template<typename OP>
inline Outcome check_not_nullptr(const OP& op)
{
	return Outcome(op != nullptr);
}

template<typename LH, typename RH>
inline Outcome check_ptr_equal(const LH* lh, const RH* rh)
{
	return Outcome(lh == rh);
}

template<typename LH, typename RH>
inline Outcome check_ptr_not_equal(const LH* lh, const RH* rh)
{
	return Outcome(lh != rh);
}

template<typename T, typename U, typename... Args>
inline Outcome check_func_returns(const T& target,
								  const U func,
								  const Args... args)
{
	auto ret = func(args...);
	return ComplexOutcome(ret == target, ret, target);
}

template<typename T, typename U, typename... Args>
inline Outcome check_func_throws(const T& target,
								 const U func,
								 const Args... args)
{
	try {
		func(args...);
	} catch (const T& e) {
		return ComplexOutcome(true, target, e);
	} catch (const std::exception& e) {
		return ComplexOutcome(false, target, e);
	} catch (...) {
		return ComplexOutcome(false, target, Nothing());	
	}

	return ComplexOutcome(false, target, Nothing());
}

#endif
