#include "goldilocks/coordinator/coordinator.hpp"
#include "iosqueak/stringy.hpp"
void Coordinator::add_find(){
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
			auto found{Coordinator::find_node(sent_string, root_node.get())};
			if(found == nullptr) continue;

			if(!found->node_name.empty()){
				std::cout<<"Found "<<found->node_name<<'\n';
			}
		}

		else if(command == "add")
		{
			std::string new_node{sent_string.back()};
			sent_string.pop_back();

			//			  Coordinator::load(new_node, sent_string);
		}

		else if(command == "print")
		{
		   //Testing to see about printing all the child nodes.
			//			  Coordinator::printChildNodes(sent_string);
		}

		else std::cout<<"Unknown command: "<<command<<'\n';
	}

	std::cout<<"Program finished\n";

}
