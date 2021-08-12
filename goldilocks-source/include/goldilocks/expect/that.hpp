/** That [Goldilocks]
 *
 * Expectation types and their evaluation functions.
 *
 * Author(s): Arsenović Arsen, Jacob Frazier, Jason C. McDonald
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

#ifndef GOLDILOCKS_THAT_HPP
#define GOLDILOCKS_THAT_HPP

#include <string> // std::toString

#include "goldilocks/expect/outcomes.hpp"

// The namespace creates the desired syntax That::IsWhatever
namespace That
{
/// Check that a value is true.
struct IsTrue {
	/// The string representation of the comparison operation taking place.
	static constexpr auto str{" == "};

	/** Performs the evaluation.
	 * \param op: the operand to check
	 * \return the outcome
	 */
	template<typename OP>
	static OutcomePtr eval(const OP& op)
	{
		return build_outcome(op == true, op, true);
	}

	/** Performs the evaluation.
	 * \param op: the pointer to the operand to check
	 * \return the outcome
	 */
	template<typename OP>
	static OutcomePtr eval(const OP* op)
	{
		// If the operand pointer is null, the test fails.
		// (We have separate checks for pointer values.)
		if (op == nullptr) {
			return build_outcome(false, nullptr, false);
		}
		// Otherwise, dereference and evaluate.
		return eval(*op);
	}
};

/// Check that a value is false.
struct IsFalse {
	/// The string representation of the comparison operation taking place.
	static constexpr auto str{" == "};

	/** Performs the evaluation.
	 * \param op: the operand to check
	 * \return the outcome
	 */
	template<typename OP>
	static OutcomePtr eval(const OP& op)
	{
		return build_outcome(op == false, op, false);
	}

	/** Performs the evaluation.
	 * \param op: the pointer to the operand to check
	 * \return the outcome
	 */
	template<typename OP>
	static OutcomePtr eval(const OP* op)
	{
		// If the operand pointer is null, the test fails.
		// (We have separate checks for pointer values.)
		if (op == nullptr) {
			return build_outcome(false, nullptr, false);
		}
		// Otherwise, dereference and evaluate.
		return eval(*op);
	}
};

/// Check that two values are equal.
struct IsEqual {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" == "};

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH& rh)
	{
		return build_outcome((lh == rh), lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH* rh)
	{
		// If either operand pointer is null, the test fails.
		// (We have separate checks for pointer values.)
		if (lh == nullptr || rh == nullptr) {
			return build_outcome(false, lh, rh);
		}
		// Otherwise, dereference and evaluate.
		return eval(*lh, *rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH& rh)
	{
		// If the operand pointer is null, the test fails.
		// (We have separate checks for pointer values.)
		if (lh == nullptr) {
			return build_outcome(false, nullptr, rh);
		}
		// Otherwise, dereference and evaluate.
		return eval(*lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH* rh)
	{
		// If the operand pointer is null, the test fails.
		// (We have separate checks for pointer values.)
		if (rh == nullptr) {
			return build_outcome(false, lh, nullptr);
		}
		// Otherwise, dereference and evaluate.
		return eval(lh, *rh);
	}
};

/// Check that two values are not equal.
struct IsNotEqual {
	/// The string representation of the comparison taking place.
	/// The string representation of the comparison taking place.
	static constexpr auto str{" != "};

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH& rh)
	{
		return build_outcome(lh != rh, lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH* rh)
	{
		if (lh == nullptr || rh == nullptr) {
			return build_outcome(false, lh, rh);
		}
		return eval(*lh, *rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH& rh)
	{
		if (lh == nullptr) {
			return build_outcome(false, nullptr, rh);
		}
		return eval(*lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH* rh)
	{
		if (rh == nullptr) {
			return build_outcome(false, lh, nullptr);
		}
		return eval(lh, *rh);
	}
};

/// Check that one value is less than the other
struct IsLess {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" < "};

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH& rh)
	{
		return build_outcome(lh < rh, lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH* rh)
	{
		if (lh == nullptr || rh == nullptr) {
			return build_outcome(false, lh, rh);
		}
		return eval(*lh, *rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH& rh)
	{
		if (lh == nullptr) {
			return build_outcome(false, nullptr, rh);
		}
		return eval(*lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH* rh)
	{
		if (rh == nullptr) {
			return build_outcome(false, lh, nullptr);
		}
		return eval(lh, *rh);
	}
};

/// Check that one value is less than or equal to the other
struct IsLessEqual {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" <= "};

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH& rh)
	{
		return build_outcome(lh <= rh, lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH* rh)
	{
		if (lh == nullptr || rh == nullptr) {
			return build_outcome(false, lh, rh);
		}
		return eval(*lh, *rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH& rh)
	{
		if (lh == nullptr) {
			return build_outcome(false, nullptr, rh);
		}
		return eval(*lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH* rh)
	{
		if (rh == nullptr) {
			return build_outcome(false, lh, nullptr);
		}
		return eval(lh, *rh);
	}
};

/// Check that one value is greater than the other
struct IsGreater {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" > "};

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH& rh)
	{
		return build_outcome(lh > rh, lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH* rh)
	{
		if (lh == nullptr || rh == nullptr) {
			return build_outcome(false, lh, rh);
		}
		return eval(*lh, *rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH& rh)
	{
		if (lh == nullptr) {
			return build_outcome(false, nullptr, rh);
		}
		return eval(*lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH* rh)
	{
		if (rh == nullptr) {
			return build_outcome(false, lh, nullptr);
		}
		return eval(lh, *rh);
	}
};

/// Check that one value is greater than or equal to the other
struct IsGreaterEqual {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" >= "};

	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH& rh)
	{
		return build_outcome(lh >= rh, lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH* rh)
	{
		if (lh == nullptr || rh == nullptr) {
			return build_outcome(false, lh, rh);
		}
		return eval(*lh, *rh);
	}

	/** Performs the evaluation.
	 * \param lh: the pointer to the left operand to check
	 * \param rh: the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH& rh)
	{
		if (lh == nullptr) {
			return build_outcome(false, nullptr, rh);
		}
		return eval(*lh, rh);
	}

	/** Performs the evaluation.
	 * \param lh: the left operand to check
	 * \param rh: the pointer to the right operand to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH& lh, const RH* rh)
	{
		if (rh == nullptr) {
			return build_outcome(false, lh, nullptr);
		}
		return eval(lh, *rh);
	}
};

/// Check that a pointer is null
struct PtrIsNull {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" == nullptr"};

	/** Performs the evaluation.
	 * \param op: the pointer to check
	 * \return the outcome
	 */
	template<typename OP>
	static OutcomePtr eval(const OP* op)
	{
		return build_outcome(op == nullptr, op, nullptr);
	}
};

/// Check that a pointer is not null
struct PtrIsNotNull {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" != nullptr"};

	/** Performs the evaluation.
	 * \param op: the pointer to check
	 * \return the outcome
	 */
	template<typename OP>
	static OutcomePtr eval(const OP* op)
	{
		return build_outcome(op != nullptr, op, nullptr);
	}
};

/// Check that two pointers point to the same address in memory.
struct PtrIsEqual {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" == "};

	/** Performs the evaluation.
	 * \param lh: the left pointer to check
	 * \param rh: the right pointer to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH* rh)
	{
		return build_outcome(lh == rh, lh, rh);
	}
};

/// Check that two pointers point to different addresses in memory.
struct PtrIsNotEqual {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" != "};

	/** Performs the evaluation.
	 * \param lh: the left pointer to check
	 * \param rh: the right pointer to check
	 * \return the outcome
	 */
	template<typename LH, typename RH>
	static OutcomePtr eval(const LH* lh, const RH* rh)
	{
		return build_outcome(lh != rh, lh, rh);
	}
};

/// Check that a function returns a given value.
struct FuncReturns {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" ==> "};

	/** Performs the evaluation.
	 * \param target: the value expected to be returned from the function
	 * \param name_hint: the name of the function as a string, which is
	 * needed for representing the outcome as a human-readable string.
	 * \param func: the function to execute
	 * \param args: the arguments to pass to the function
	 * \return the outcome
	 */
	template<typename T, typename U, typename... Args>
	static OutcomePtr eval(const T& target,
						   const char* name_hint,
						   const U func,
						   const Args... args)
	{
		(void)name_hint;  // TODO: Process name hint
		auto ret = func(args...);
		// TODO: Store function call data in outcome!
		return build_outcome(ret == target, ret, target);
	}
};

/// Check that a function raises a given exception.
struct FuncThrows {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" ~~> "};

	/** Performs the evaluation.
	 * \param target: the exception (or other object) expected to be thrown
	 * \param name_hint: the name of the function as a string, which is
	 * needed for representing the outcome as a human-readable string.
	 * \param func: the function to execute
	 * \param args: the arguments to pass to the function
	 * \return the outcome
	 */
	template<typename T, typename U, typename... Args>
	static OutcomePtr eval(const T& target,
						   const char* name_hint,
						   const U func,
						   const Args... args)
	{
		(void)name_hint;  // TODO: Process name_hint
		try {
			func(args...);
		} catch (const T& e) {
			// TODO: Store function call data in outcome!
			return build_outcome(true, target, e);
		} catch (const std::exception& e) {
			return build_outcome(false, target, e);
		} catch (...) {
			return build_outcome(false, target, Nothing());
		}

		return build_outcome(false, target, Nothing());
	}

	// "Throw Nothing" version of eval()
	template<typename U, typename... Args>
	static OutcomePtr eval(const Nothing& target,
						   const char* name_hint,
						   const U func,
						   const Args... args)
	{
		(void)name_hint;  // TODO: Process name_hint
		try {
			func(args...);
		} catch (...) {
			return build_outcome(false, Nothing(), target);
		}

		return build_outcome(true, target, Nothing());
	}
};

// Expects value to be in the inclusive range defined by lower and upper.
struct IsInRange {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" ∈ "};
	
	/** Performs the evaluation.
	 * \param value: the reference to the value operand to check.
	 * \param lower: the pointer to minimum range defined.
	 * \param upper: the pointer to maximum range defined.
	 * \return the outcome
	 */
	template <typename T, typename U, typename V>
	static OutcomePtr eval(const T& value, const U& lower, const V& upper) {
		return build_outcome(value >= lower && value <= upper, 
			value, 
			"{" + std::to_string(lower) + ", " + std::to_string(upper) + "}");
	}

	/** Performs the evaluation.
	 * \param value: the reference to the value operand to check.
	 * \param lower: the minimum range defined.
	 * \param upper: the maximum range defined.
	 * \return the outcome
	 */
	template<typename T, typename U, typename V>
	static OutcomePtr eval(const T* value, const U& lower, const V& upper) {
		if (value == nullptr) {
			return build_outcome(false, 
			nullptr, 
			"{" + std::to_string(lower) + ", " + std::to_string(upper) + "}");
		}
		return eval(*value, lower, upper);
	}
};

// Expects value to be outside the inclusive range defined by lower and upper.
struct IsNotInRange {
	/// The string representation of the comparison taking place.
	static constexpr auto str{" ∉ "};

	/** Performs the evaluation.
	 * \param value: the reference to the value operand to check.
	 * \param lower: the pointer to minimum range defined.
	 * \param upper: the pointer to maximum range defined.
	 * \return the outcome
	 */
	template<typename T, typename U, typename V>
	static OutcomePtr eval(const T& value, const U& lower, const V& upper) {
		return build_outcome(value < lower || value > upper, 
		value, 
		"{" + std::to_string(lower) + ", " + std::to_string(upper) + "}");
	}

	/** Performs the evaluation.
	 * \param value: the reference to the value operand to check.
	 * \param lower: the minimum range defined.
	 * \param upper: the maximum range defined.
	 * \return the outcome
	 */
	template<typename T, typename U, typename V>
	static OutcomePtr eval(const T* value, const U& lower, const V& upper) {
		if (value == nullptr) {
			return build_outcome(false, 
			nullptr, 
			"{" + std::to_string(lower) + ", " + std::to_string(upper) + "}");
		}
		return eval(*value, lower, upper);
	}
};

}  // namespace That

#endif
