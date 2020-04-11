#include <iostream>

#include "Callable.h"
#include "Automaton.h"

Automaton::Automaton(std::string name, std::string modulePath)
	: name(name), modulePath(modulePath), handle(), nodes()
{
	current_state = getNode("_start_");
}

std::string Automaton::getName() const
{
	return name;
}

std::shared_ptr<AutomatonNode> Automaton::getNode(std::string nodeName)
{
	if( nodes.count(nodeName)==0 )
	{
		std::shared_ptr<AutomatonNode> newNode = std::make_shared<AutomatonNode>(nodeName);
		nodes[nodeName] = newNode;
	}
	return nodes[nodeName];
}

int Automaton::inputCommand(std::string command, std::string argument)
{
	current_state = current_state->inputCommand(command, argument);
	return 0;
}

int Automaton::loadBinary()
{
	handle =  LoadLibrary(modulePath.c_str());
	if (!handle)
	{
	    std::cerr << "could not load the dynamic library " << modulePath << std::endl;
	    return EXIT_FAILURE;
	}

	for(auto& node: nodes)
	{
		_callable call = (_callable)GetProcAddress(handle, node.first.c_str());
		if (!call)
		{
			std::cerr << "could not locate the function " << node.first << " using default callable" << std::endl;
			continue;
		}
		node.second->setCallable(call);
	}

	return 0;
}
