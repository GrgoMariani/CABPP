#include "AutomatonNodeDirected.h"

#include <iostream>

#include "Callable.h"

AutomatonNodeDirected::AutomatonNodeDirected(std::string name) : AutomatonNodeBase(name), edges()
{

}

AutomatonNodeDirected::~AutomatonNodeDirected()
{

}


void AutomatonNodeDirected::connectCommand(std::string command, std::shared_ptr<AutomatonNodeBase> nextNode)
{
	edges[command] = std::static_pointer_cast<AutomatonNodeDirected>(nextNode);
}


std::shared_ptr<AutomatonNodeBase> AutomatonNodeDirected::inputCommand(std::string command, std::string argument)
{
	if(edges.count(command) == 0)
		return shared_from_this();
	std::shared_ptr<AutomatonNodeDirected> next_node = edges[command];
	try{
		next_node->callCallable(argument);
	}
	catch(std::exception& e)
	{
		std::cerr << "EXCEPTION AT " << next_node->getName() << ":::::" << e.what() << std::endl;
	}

	return std::static_pointer_cast<AutomatonNodeBase>(next_node);
}
