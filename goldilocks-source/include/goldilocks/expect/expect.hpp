/** Assert [Goldilocks]
 * Version: 2.0
 *
 * Assert functionality for Goldilocks.
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

#ifndef GOLDILOCKS_EXPECT_HPP
#define GOLDILOCKS_EXPECT_HPP

#include <iostream>

#include "goldilocks/expect/outcomes.hpp"
#include "goldilocks/expect/should.hpp"
#include "goldilocks/expect/that.hpp"
#include "goldilocks/types.hpp"
#include "iosqueak/stringify.hpp"

/// The smallest unit of a test, representing a single check.
template<typename H, Should S = Should::Pass> class Expect
{
protected:
	/// The outcome of the check.
	OutcomePtr outcome;

public:
	/// Default constructor, which also executes the check.
	template<typename... Ts>
	// cppcheck-suppress noExplicitConstructor
	Expect(Ts... ts) : outcome(H::eval(ts...))
	{
	}

	/** Represent the outcome of the check as a string.
	 * \return a string representing the outcome
	 */
	testdoc_t compose() const
	{
		/* Conversion to a string should take place on demand,
		 * not at the time of testing. */
		// Create the string representation of the outcome.
		return this->outcome->compose(S, H::str);
	}
};

/// Insert the string representation of an expectation outcome into a stream.
template<typename H, Should S = Should::Pass>
std::ostream& operator<<(std::ostream& out, const Expect<H, S>& expect)
{
	// Insert the string representing the outcome into the stream.
	out << expect.compose();
	return out;
}

#endif
