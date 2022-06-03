#include "../include/goldilocks/coordinator.hpp"

/* Adds a new child to the current node. It calls find_node to check if
 * on_node is empty. If so, root is the parent, otherwise calls find_node.
 */
void Coordinator::load(const std::string& new_child, std::vector<std::string>& parent)
{
    std::shared_ptr<Node> new_node{root};

    //If parent is empty, add child to root node.
    if(parent.empty())
    {
        root->add_child(new_child);
    }

    else
    {   //Iterate through tree, add any nodes needed.
        std::for_each(parent.begin(), parent.end(), [&new_node](const auto& node)
        {
            /*If returned value is nullptr, create new node.
            * Change new_node to newly created node
            */
            if(new_node->find_child(node) == nullptr)
            {
                //Add child to new_node vector.
                new_node->add_child(node);

                //Switch new_node to the child just created.
                new_node = new_node->find_child(node);
            }
            //If child was found, make the child node the new_node.
            else
            {
                new_node = new_node->find_child(node);
            }
        });

		//Make new_child the child of the last node.
        new_node->add_child(new_child);
    }
}

/* Looks for node with node_path value. Returns a shared_ptr if found. */
std::shared_ptr<Node> Coordinator::find_node(std::vector<std::string>& node_path)
{
    auto current_node{root};
    current_node->printChildren(current_node);

    //If string is empty, return the root node.
    if(node_path.empty())
    {
        return current_node;
    }

    for(auto& token: node_path)
    {
		//Make next_node the token in the iteration.
        std::shared_ptr<Node> next_node{current_node->find_child(token)};

        // If returned node from next_node is nullptr and vector only has one element, print message and return.
        if (next_node == nullptr && node_path.size() == 1) {
            std::cout << "Cannot find node " << token << " on "<< current_node->node_name << '\n';
            return current_node;
        }

        // If next_node is nullptr, return the current node.
        if(next_node == nullptr) return current_node;

        // If next_node has a value, make current_node equal next_node.
        if (next_node->node_name == token) {
            current_node = next_node;
        }

    }

    return current_node;
}

void Coordinator::printChildNodes(std::vector<std::string>& nodes)
{
    for (auto& child: nodes)
    {
        std::vector<std::string> temp{child};
        root->printChildren(this->find_node(temp));
    }

}
