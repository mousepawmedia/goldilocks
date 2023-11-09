#include "goldilocks/coordinator.hpp"

//Forward declare process_tests.
//void add_tests(std::string_view, node_ptr);
void add_tests(std::string_view, Node<Suites>::shr_nodeptr);

//bool Coordinator::add_children(node_ptr node)
bool Coordinator::add_children(Node<Suites>::shr_nodeptr node)
{
    /* Add all of the Suites/Tests in Test_List class to the
     * proper containers with dynamic_cast check. */
    for(auto& test: this->testlist.send_tests())
    {
        // std::string path{test.get()->node_name};
        commands root{node->node_name};

        // if(is_test(test.get()))
        if(dynamic_cast<Tests*>(node.get()) != nullptr)
        {
            // this->add_node(test->node_name, root, node);
            add_tests(test->node_name, std::make_shared<Tests>(static_cast<Tests*>(test.get())));
        }

        if(dynamic_cast<Suites*>(test.get()) != nullptr)
        {
            // auto suites{std::make_shared<Suites>(static_cast<Suites*>(test.get())->node_name)};
            
            if(!static_cast<Suites*>(test.get())->suites_send_tests().empty())
            // if(!suites->suites_send_tests().empty())
            {
                // if(!suites.get()->suites_send_tests().empty())
                // {
                    // for(auto& tests: suites.get()->suites_send_tests())
                    for(auto& suite_tests: static_cast<Suites*>(test.get())->suites_send_tests())
                    {
                        root.push_back(tests->name);
                        // Coordinator::add_node(tests.get()->name, root, suites.get());
                        Coordinator::add_node(suite_tests.get()->name, root, suites.get());
                        root.pop_back();
                    }
                }
            // }
        }
        else
        {
            channel <<node->node_name << " is not a Suites or Tests class\n"<< IOCtrl::endl;
        }
    }   
    
    return 1;
}

// Process the tests sent from add_children function.
void add_tests(std::string_view, Node<Suites>::shr_nodeptr)
{
    node.get()->children.push_back();
}

void insert_node(std::string_view name, Coordinator::node_ptr node)
{
    (void) name; (void) node;
}


        // available_suites[test.get()->node_name] = std::make_shared<Suites>(static_cast<Suites*>(test.get())->node_name);

        // this->available_tests[test.get()->node_name] = std::make_shared<Tests>(static_cast<Tests*>(test.get())->node_name);
        // auto testing {dynamic_cast<Tests*>(test.get())};
