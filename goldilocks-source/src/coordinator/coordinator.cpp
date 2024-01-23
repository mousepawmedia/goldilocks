#include "goldilocks/coordinator/coordinator.hpp"

Coordinator::Coordinator()
{
   {
        /* A group of Tests and Suites. */
        Test_List suites_tests;

        std::cout<<"Starting to fill tree.\n";

        for(auto& suite: root->children){
//            Coordinator::fill_tree(suite->children, root);
//            Coordinator::fill_tree(suite->suites_container, root);
            std::cout<< suite->node_name << '\n';
        }

//        Coordinator::fill_tree(suites_tests.main_suite.suites_container, root);
//        Coordinator::fill_tree(suites_tests.main_suite.tests_container, root);
    }
std::cout<<"Ending fill_tree\n";
        Coordinator::print_tree(root);
}

