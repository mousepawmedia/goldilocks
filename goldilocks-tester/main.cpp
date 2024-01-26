/** Goldilocks Tester
 * Version: 2.0
 *
 * Allows running tests and custom code for Goldilocks.
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

#include <iostream>

#include "goldilocks/expect/expect.hpp"
#include "iosqueak/channel.hpp"
#include "iosqueak/stringy.hpp"
#include "goldilocks/coordinator/coordinator.hpp"
#include "goldilocks/coordinator/node.hpp"
#include "goldilocks/goldilocks_shell.hpp"

bool griffon(std::string, int) { return true; }

bool explosion() { throw std::runtime_error("Foo"); }

void black_hole(int) { return; }

/** Temporary test code goes in this function ONLY.
 * All test code that is needed long term should be
 * moved to a dedicated Goldilocks Test and Suite.
 */

void test_code()
{
//	Node root{"root"};
//	std::unique_ptr<Node> first{std::make_unique<Node>("first")};
//	std::unique_ptr<Node> second{std::make_unique<Node>("second")};
//	root.children.emplace_back(std::move(first));
//	root.children.emplace_back(std::move(second));
//	std::cout<<std::boolalpha;
//	std::cout<< "Does root have first? " <<root.has_child("first") <<'\n';
//	std::unique_ptr<Node> third{root.get_child("first")};
//	std::cout<< "Does root have first? " <<root.has_child("first") <<'\n';

//	Goldilocks_Shell gold{"golden"};

//    Coordinator root;



}


/////// WARNING: DO NOT ALTER BELOW THIS POINT! ///////

int main(int argc, char* argv[])
{
	// Return code.
	int r = 0;

	// Set up signal handling.
	channel.configure_echo(IOEchoMode::cout);

	/****
	GoldilocksShell* shell = new GoldilocksShell(">> ");
	//shell->register_suite<Suite_CoreTypes>("P-sB01");

	// If we got command-line arguments.
	if(argc > 1)
	{
		r = shell->command(argc, argv);
	}
	else
	{
		out << IOFormatTextAttr::bold << IOFormatTextFG::blue
			<< "===== Goldilocks Tester =====\n" << IOCtrl::endl;

		test_code();

		// Shift control to the interactive console.
		shell->interactive();
	}

	// Delete our GoldilocksShell.
	delete shell;
	****/

	// HACK: Remove me before final commit.
	(void)argc;
	(void)argv;
	test_code();

	return r;
}
