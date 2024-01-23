#include "coordinator.hpp"



// Function to add test_container elements to tree.
void process_test_container(Coordinator::shrptr_suite node);

// Temporary function to process string into vector. Delete when done.
std::deque<std::string> process_path(std::string& path);

void Coordinator::fill_tree(std::vector<std::shared_ptr<BaseNode>>& container, std::shared_ptr<Suites> parent) {
    for (const auto& item : container) {
        if (std::shared_ptr<Suites> suite = std::dynamic_pointer_cast<Suites>(item)) {
            parent->add_suite(suite);
            auto test{dynamic_cast<Suites>(item)};
            fill_tree(suite, suite);
            fill_tree(suite->, suite);
        } else if (std::shared_ptr<Tests> test = std::dynamic_pointer_cast<Tests>(item)) {
            parent->add_test(test);
        }
    }
}

//void Coordinator::run_tests() {
//    fill_tree(suite->suites_ctn, suite);
//    fill_tree(suite->tests_ctn, suite);
//    suite->print_tree();
//}

//void Coordinator::fill_tree(std::string_view name, Suites::suites_ctn& suites, Node::shr_nodeptr node)
//void Coordinator::fill_tree(std::string_view name, Suites::suites_ctn& suites, Suites::shr_suite node)
//{
//std::cout<<"Running fill_tree with three params\n";
//std::cout<<"Path: "<<name  <<'\n';
//    std::string path{std::string(name)};
//    auto        tokens{process_path(path)};
////std::cout<< "first token: " <<tokens.front()<<'\n';
//    std::cout<<"First token: "<<tokens.front()<< " and node name: "<< node->name <<'\n';
//    auto searched_node{std::find_if(suites.begin(),suites.end(), [&tokens](auto& node_){ return tokens.front() == node_->name;})};
//    if(searched_node != suites.end())
//    {
//        std::cout<<"Searched+node name: "<<searched_node->get()->name <<'\n';
//        tokens.pop_front();
//        node = *searched_node;
//        suites = searched_node->get()->suites_container;

//        tokens.push_back(std::string(node->name));
//        path = std::string();

//        for(auto& token: tokens)
//        {
//            // If not the last token, add '.' to word, else just the token.
//            (&token != &tokens.back())? path += token + "." : path += token;
//        }
//        Coordinator::fill_tree(path, node->suites_container, node);

//    }

//    else if(searched_node == suites.end())
//    {
//        std::cout<<node->name <<" was found and being added to tree\n";
//        Coordinator::add_node(node, node);

//        path = std::string();
//        for(auto& token: tokens)
//        {
//            // If not the last token, add '.' to word, else just the token.
//            (&token != &tokens.back())? path += token + "." : path += token;
//        }
//std::cout<<"Path after search end: "<<path<<'\n';

//        for(auto& suite: suites)
//        {
//            if(!suite->tests_container.empty())
//            {
//                Coordinator::fill_tree(suite->tests_container, node);
//            }
//            if(!suite->suites_container.empty())
//            {
//                Coordinator::fill_tree(path, suite->suites_container, node);
//            }
//        }
//    }

////    Coordinator::fill_tree(path, node->suites_container, node);
////    auto searched_node{std::find_if(suites.begin(), suites.end(),[&tokens](auto& name){return tokens.front() == name;})};


//}

//void Coordinator::fill_tree(Suites::tests_ctn& tests,  Node::shr_nodeptr node)
//{

//    for(auto& test: tests){
//std::cout<<"Running fill_tree tests\n";
//        node->children.emplace_back(std::make_shared<Suites>(node->name));
//        node->children.back()->name = node->name;
//        node->children.back()->func = node->func;
//    }

//}

std::deque<std::string> process_path(std::string& path)
{
    std::deque<std::string>     vec;
    std::string                 word;

    std::for_each(path.begin(), path.end(), [&vec, &word](char ch)
    {
        if(ch != '.') word.push_back(ch);
        else{
            vec.emplace_back(word);
            word = std::string();
        }
     });

    vec.emplace_back(word);

    return vec;
}


////void Coordinator::fill_tree(std::string_view name, Node::shr_nodeptr node)
////{

////    std::cout<<"Running fill_tree suites\n";
////    for(auto& suite: node->suites_container)
////    {
////        Coordinator::add_node(suite, node);
////        if(!node->suites_container.empty())
////        {
////            Coordinator::fill_tree(name, suite);
////        }
////    }
////}


////    if(tokens.size()){
////        tokens.pop_front();
////        auto searched_node = std::find_if(node->children.begin() , node->children.end(), [&tokens](auto& node_){return tokens.front() == node_->name;});

////        if(searched_node != node->children.end())
////        {
////            path = std::string();
////std::cout<< "first token after pop: " <<tokens.front()<<'\n';
////            if(!tokens.empty())
////            {
////                for(auto& token: tokens)
////                {
////                    // If not the last token, add '.' to word, else just the token.
////                    (&token != &tokens.back())? path += token + "." : path += token;
////                }
////            }
////            else
////            {
////                path = node->name;
////            }
////std::cout<<path<<'\n';
////            std::cout<< searched_node->get()->name <<" was already in the tree\n";
////            Coordinator::fill_tree(path, searched_node->get()->suites_container, *searched_node);

////        }
////        else if(searched_node == node->children.end())
////        {
////            if(!suites.empty())
////            {
////                for(auto& suite: suites)
////                {
////                    // Add suite to current node.
////                    Coordinator::add_node(suite, node);

////                    // Check for tests in test_container.
////                    if(!suites.front()->tests_container.empty())
////                    {
////                        Coordinator::fill_tree(suites.front()->tests_container, node);
////                    }
////                    Coordinator::fill_tree(path + "." + std::string(suite->name), suite->suites_container, node);
////                }
////            }
////        }
////    }
