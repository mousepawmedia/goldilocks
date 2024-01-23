#include "coordinator.hpp"

void tree_things(std::shared_ptr<Node> suite);
//void tree_things(std::shared_ptr<Node> suite);
void test_things(std::shared_ptr<Node> test);
//void test_things(std::shared_ptr<Node> test);

//void Coordinator::print_tree(Node node)
//void Coordinator::print_tree(Node node)
//{
//    for(auto& suite: node.children)
//    {
//        if(suite->func != NULL)
//        {
//            std::cout<<"node name: "<<node.name<<'\n';
//            test_things(suite);
//        }
//        else
//        {
//            tree_things(suite);
//        }
//    }
//}

//void tree_things(std::shared_ptr<Node> suite)
//{
//    if(suite == nullptr) return;

//    if(!suite->children.empty())
//    {
//        std::cout<<"Tests in "<<suite->name <<":\n";
//        for(auto& test: suite->children)
//        {
////            std::cout<< test->name << '\n';
//            std::cout<< test << '\n';
//        }
//    }
//}

//void test_things(std::shared_ptr<Node> test)
//{
//    if(test == nullptr) return;
//    std::cout<<"test name: "<<test->name<<'\n';

//}
// Function to print the tree
#include "node.hpp"
void Coordinator::print_tree(const Node::shr_nodeptr& node, int depth) {
    if (!node) {
        return;
    }

    // Indentation based on depth
    std::string indentation(depth * 4, ' ');

    // Print node name
    std::cout << indentation << "- " << node->name << std::endl;

    // Print children recursively
    for (const auto& child : node->children) {
        print_tree(child, depth + 1);
    }
}
