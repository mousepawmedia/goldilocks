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

////#include "goldilocks/goldilocks_shell.hpp"
#include <iostream>

#include "goldilocks/expect/expect.hpp"
#include "iosqueak/channel.hpp"
#include "goldilocks/coordinator.hpp"

bool griffon(std::string, int) { return true; }

bool explosion() { throw std::runtime_error("Foo"); }

void black_hole(int) { return; }

/** Temporary test code goes in this function ONLY.
 * All test code that is needed long term should be
 * moved to a dedicated Goldilocks Test and Suite.
 */

int add(int a, int b) { return a + b; }

void test_code()
{
/*	/// Checks if two float numbers are approximately within a margin
	std::cout << Expect<That::IsApproxEqual>(10.003f, 10.001f, 3) << std::endl;
	std::cout << Expect<That::IsApproxEqual>(0.003f, 0.03f, 1) << std::endl;
	std::cout << Expect<That::IsApproxEqual>(2.5f, 2.6f, 1) << std::endl;

	/// Checks if two float numbers are not approximately within a margin
	std::cout << Expect<That::IsApproxNotEqual>(10.003f, 10.001f, 0)
			  << std::endl;
	std::cout << Expect<That::IsApproxNotEqual>(0.003f, 0.03f, 0) << std::endl;
	std::cout << Expect<That::IsApproxNotEqual>(3.5f, 4.8f, 1) << std::endl;

	// Testing if the rest of the features are working just fine with the new
	// changes
	std::cout << Expect<That::IsInRange>(5, 1, 2) << std::endl;
	std::cout << Expect<That::FuncReturns>(5, "add", add, 2, 3) << std::endl;
*/
    Coordinator root;

    while(true)
    {
        std::cout<<"<find|add> node (or quit): ";
        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> sent_string;
        stringy::split_string(input, " ", sent_string);

        std::string command{sent_string.front()};
        sent_string.erase(sent_string.begin());

        stringy::split_string(sent_string.front(), ".", sent_string);
        sent_string.erase(sent_string.begin());

        if(command == "quit"){
            sent_string.push_back("");
            break;
        }

        else if(command == "find")
        {
            auto found{root.find_node(sent_string)};
            if(found == nullptr) continue;

            if(!found->node_name.empty()){
                std::cout<<"Found "<<found->node_name<<'\n';
            }
        }

        else if(command == "add")
        {
            std::string new_node{sent_string.back()};
            sent_string.pop_back();

            root.load(new_node, sent_string);
        }

        else if(command == "print")
        {
            //Testing to see about printing all the child nodes.
            root.printChildNodes(sent_string);
        }

        else std::cout<<"Unknown command: "<<command<<'\n';
    }

    cout<<"Program finished\n";


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
