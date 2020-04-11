#include <iostream>

#include "AutomatonNode.h"
#include "Callable.h"

AutomatonNode::AutomatonNode(std::string name) : name(name), callable(default_callable), edges()
{

}

std::string AutomatonNode::getName() const
{
	return name;
}

void AutomatonNode::connectCommand(std::string command, std::shared_ptr<AutomatonNode> nextNode)
{
	edges[command] = nextNode;
}

int AutomatonNode::setCallable(_callable newCallable)
{
	callable = newCallable;
	return 0;
}


int AutomatonNode::callCallable(std::string argument)
{
	return callable(argument);
}


std::shared_ptr<AutomatonNode> AutomatonNode::inputCommand(std::string command, std::string argument)
{
	if(edges.count(command) == 0)
		return shared_from_this();
	std::shared_ptr<AutomatonNode> next_node = edges[command];
	try{
		next_node->callCallable(argument);
	}
	catch(std::exception& e)
	{
		std::cerr << "EXCEPTION AT " << next_node->getName() << ":::::" << e.what() << std::endl;
	}
	return next_node;
}
