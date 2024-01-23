#include "coordinator.hpp"

//Node::shr_nodeptr Coordinator::find_node(std::deque<std::string>& node_path, Node::shr_nodeptr node)
Node::shr_nodeptr Coordinator::find_node(std::string_view name, Node::shr_nodeptr node)
{
    auto searched_node = std::find_if(node->children.begin(), node->children.end(), [&name](auto& node_){return name == node_->name;});
    return (searched_node != std::end(node->children))? *searched_node: nullptr;
}

std::shared_ptr<Tests> Coordinator::find_node(std::string_view name, std::shared_ptr<Tests> node)
{
    //Pointless function????
    std::cout<<"Usless find_node for tests function called\n";
    return nullptr;
}


////    Node::shr_nodeptr new_node{std::make_shared<Suites>("new_node")};
//    while(!node_path.empty())
//    {
//        std::cout<<"find_node node name: "<< node->name<<'\n';
//        auto search_node{std::find_if(node->children.begin(), node->children.end(), [&node_path] (auto& searched_node){
//        return searched_node->name == node_path.front();})};

//        if(search_node != node->children.end()){
//           node = *search_node;
//            std::cout<<"find_node "<<search_node->get()->name <<" was in tree\n";
//            node_path.erase(node_path.begin());
//            continue;
//        }
//        else{
//            std::cout<<"find_node returning nullptr\n";
//            return nullptr;
//        }
//    }
//    return node;
