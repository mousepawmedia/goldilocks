#include "goldilocks/coordinator.hpp"

// bool Coordinator::add_child(std::string_view name, node_ptr node){
bool Coordinator::add_child(const std::string& name, node_ptr node){
    (void) name; (void) node;
    return 1;
}
