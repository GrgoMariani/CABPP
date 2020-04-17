#include "AutomatonNodeFullyConnected.h"

#include <iostream>

#include "Callable.h"

AutomatonNodeFullyConnected::AutomatonNodeFullyConnected(std::string name, std::shared_ptr<std::map<std::string, std::shared_ptr<AutomatonNodeFullyConnected>>> edges)
	: AutomatonNodeBase(name), edges(edges)
{

}

AutomatonNodeFullyConnected::~AutomatonNodeFullyConnected()
{

}


void AutomatonNodeFullyConnected::connectCommand(std::string command, std::shared_ptr<AutomatonNodeBase> nextNode)
{
	(*edges)[command] = std::static_pointer_cast<AutomatonNodeFullyConnected>(nextNode);
}


std::shared_ptr<AutomatonNodeBase> AutomatonNodeFullyConnected::inputCommand(std::string command, std::string argument)
{
	if(edges->count(command) == 0)
		return shared_from_this();
	std::shared_ptr<AutomatonNodeFullyConnected> next_node = (*edges)[command];
	try{
		next_node->callCallable(argument);
	}
	catch(std::exception& e)
	{
		std::cerr << "EXCEPTION AT " << next_node->getName() << ":::::" << e.what() << std::endl;
	}

	return std::static_pointer_cast<AutomatonNodeBase>(next_node);
}
