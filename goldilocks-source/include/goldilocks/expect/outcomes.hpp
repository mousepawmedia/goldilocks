/** Outcomes [Goldilocks]
 *
 * Expectation outcomes.
 *
 * Author(s): Jacob Frazier, Jason C. McDonald
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

#ifndef GOLDILOCKS_OUTCOMES_HPP
#define GOLDILOCKS_OUTCOMES_HPP

#include "goldilocks/expect/should.hpp"
#include "goldilocks/types.hpp"
#include "iosqueak/stringify.hpp"

/// Common interface for all outcomes.
class AbstractOutcome
{
protected:
	/// Whether the associated evaluation was successful
	bool pass;

	// Disallow instantiating AbstractOutcome directly.
	explicit AbstractOutcome(const bool& pass) : pass(pass) {}

public:
	/** Report whether the evaluation was successful.
	 * \param should: how success should be interpreted
	 * \return true if successful, else false
	 */
	virtual bool success(const Should& should) const final
	{
		switch (should) {
			// If success implies the evaluation should have succeeded...
			case Should::Pass:
				[[fallthrough]];
			case Should::Pass_Silent:
				// Report the evaluation's success state directly
				return pass;
			// If success implies the evaluation should have failed...
			case Should::Fail:
				[[fallthrough]];
			case Should::Fail_Silent:
				// Report the inverse of the evaluation's success state.
				return !pass;
		}
	}

	/// Create the string representation of the outcome. (Abstract)
	virtual testdoc_t compose(const Should&,
							  const testdoc_t comparison) const = 0;

	virtual ~AbstractOutcome() = default;
};

/** A pointer to an outcome.
 * Serves as a universal "handle" for working with outcomes,
 * regardless of their type, complexity, or parameter types.
 */
typedef std::shared_ptr<AbstractOutcome> OutcomePtr;

/// Outcome of evaluation for a typical binary expression.
template<typename X, typename R>
class Outcome : public AbstractOutcome
{
protected:
	/// The left-hand operand, or actual value.
	R returned;
	/// The right-hand operand, or expected value.
	X expected;

public:
	/** Create a new outcome.
	 * \param passed: the raw outcome of the comparison as a boolean
	 * \param expected: the expected value (or right-hand operand)
	 * \param returned: the actual value (or left-hand operand)
	 * \return an outcome object
	 */
	Outcome<X, R>(const bool& passed, const R& returned, const X& expected)
	: AbstractOutcome(passed), returned(returned), expected(expected)
	{
	}

	/** Create the string representation of the outcome.
	 * \param should: how success should be interpreted.
	 * \param comparison: the string representing the comparison operation
	 */
	testdoc_t compose(const Should& should,
					  const testdoc_t comparison) const override
	{
		// TODO: Potentially drop in favor of more robust formatting elsewhere

		// Create the appropriate outcome representation string.
		testdoc_t outcome = "";
		switch (should) {
			case Should::Pass_Silent:
				if (this->pass) {
					return "";
				}
				[[fallthrough]];
			case Should::Pass:
				if (this->pass) {
					outcome = " [PASS]";
				} else {
					outcome = " [FAIL]";
				}
				break;
			case Should::Fail_Silent:
				if (!this->pass) {
					return "";
				}
				[[fallthrough]];
			case Should::Fail:
				if (!this->pass) {
					outcome = " [PASS: Failed]";
				} else {
					outcome = " [FAIL: Passed]";
				}
				break;
		}

		// Compose a string from the value, comparison, and outcome strings.
		return stringify(returned) + comparison + stringify(expected) + outcome;
	}

	~Outcome() = default;
};

/** Build an outcome.
 * \param passed: the raw outcome of the comparison as a boolean
 * \param expected: the expected value (or right-hand operand)
 * \param returned: the actual value (or left-hand operand)
 * \return a shared pointer to the outcome.
 */
template<typename X, typename R>
inline OutcomePtr build_outcome(const bool& passed,
								const R& returned,
								const X& expected)
{
	return std::make_shared<Outcome<X, R>>(passed, returned, expected);
}

#endif
