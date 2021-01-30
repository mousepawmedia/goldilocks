/** Should [Goldilocks]
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

#ifndef GOLDILOCKS_SHOULD_HPP
#define GOLDILOCKS_SHOULD_HPP

#include <string>

/// Determines whether the evaluation is expected to pass or fail.
enum class Should { Pass, Pass_Silent, Fail, Fail_Silent };

/** Converts a Should value to a string.
 * \param should: the Should value to convert
 * \return a string representing the Should value.
 */
std::string stringify(const Should& should)
{
	/* This function is here to provide stringify() support less expensively
	 * than overloading operator<<.
	 */

	/* Should::Pass is the normal expectation, and so can be unstated.
	 * We also won't print something different to indicate Silent modes.
	 * This only needs to return a value for unusual cases, e.g. Should::Fail.
	 */
	switch (should) {
		case Should::Pass:
		case Should::Pass_Silent:
			return "";
		case Should::Fail:
		case Should::Fail_Silent:
			return "[Should Fail]";
	}
	return "";
}

#endif
