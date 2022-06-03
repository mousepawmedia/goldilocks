/** Coordinator [Goldilocks]
 *
 * Author(s): Jack Tolmie, Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2022 MousePaw Media.
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

#ifndef GOLDILOCKS_COORDINATOR_HPP
#define GOLDILOCKS_COORDINATOR_HPP

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string_view>
#include <vector>
#include "iosqueak/stringy.hpp"
using std::cout;

// Useless class. Only for testing. Delete when done.
class Runnable
{
public:
// cppcheck-suppress noExplicitConstructor
	Runnable(std::string_view name)
	{
		std::cout << "Runnable called as: " << name << "\n";
	}
};

class Node
{
    // Shared pointer to runnable sent with the caller.
    std::shared_ptr<Runnable> runnable{};

    /* A vector to hold the children of the node that contains tests or
     *  other suites.
     */
    std::vector<std::shared_ptr<Node>> children;

public:
    std::string node_name{};

    // Node constructor.
    // cppcheck-suppress noExplicitConstructor
    Node(std::string_view name) : runnable{new Runnable(name)}, node_name{name}  {}

    /* Function to find children in the vector m_children. Returns a
     * shared_ptr with the node if found, otherwise shared_ptr of nullptr.
     */
    std::shared_ptr<Node> find_child(std::string_view child_name)
    {
        auto child{find_if(children.begin(), children.end(), [child_name](auto& name){return name->node_name == child_name;})};
        return (child != children.end())? *child : std::shared_ptr<Node>(nullptr);
    }

    // A function to add a child node to m_children.
    void add_child(std::string_view name)
    {
        this->children.emplace_back(new Node(name));
    }

    /*Just a test to print the children of each node. It does not work fully in the sense of
    * it prints them, but it is not formatted nicely. Need to find a way to make it print nicer
    * if we want to keep this.*/

    void printChildren(std::shared_ptr<Node> node){
        if(node->children.empty())
        {
            cout<<node->node_name +" has no children"<<'\n';
            return;
        }
        for(size_t child{0}; child < node->children.size(); child++)
        {
            if(child == 0)
            {
                cout<<std::setw(40 + (node->children[child]->node_name.size() / 2))<<node->children[child]->node_name<<'\n';
                for(size_t putTab{0}; putTab < node->children.size(); putTab++)
                {
                    cout<<std::setw(40 )<<"|\n";
                }

                for(auto& loop_child : node->children[child]->children)
                {
                    cout<<loop_child->node_name<< ' ';
                }
                cout<<'\n';
            }
            else
            {
                for(size_t putTab{0}; putTab < node->children.size(); putTab++)
                {
                    cout<<std::setw(node->children[putTab]->node_name.size() / 2)<<"|\t";
                }
                    cout<<'\n'<<std::setw(node->children[child]->node_name.size() )<<"\t"<<node->children[child]->node_name <<'\t';
            }
        }
    }
};

class Coordinator
{
    // Default shared_ptr. Created to make it work with the member functions easier.
    std::shared_ptr<Node> root{std::make_shared<Node>("root")};

    std::string_view name;

public:
    /* Adds a new child to the current node. It calls find_node to check if
     * on_node is empty. If so, root is the parent, otherwise calls find_node.
     */
    void load(std::string& name, std::vector<std::string>& parent);

    std::shared_ptr<Node> find_node(std::vector<std::string>& node_path);

    void printChildNodes(std::vector<std::string>& nodes);
};

#endif  // !GOLDILOCKS_COORDINATOR_HPP
