#include <iostream>

#include "AutomatonFullyConnected.h"

AutomatonFullyConnected::AutomatonFullyConnected(std::string name, std::string modulePath)
	: AutomatonBase(name, modulePath)
{
	nodes = std::make_shared<std::map<std::string, std::shared_ptr<AutomatonNodeFullyConnected>>>();
	current_state = getNode("_start_");
}

AutomatonFullyConnected::~AutomatonFullyConnected()
{

}

std::shared_ptr<AutomatonNodeBase> AutomatonFullyConnected::getNode(std::string nodeName)
{
	if( nodes->count(nodeName)==0 )
	{
		std::shared_ptr<AutomatonNodeFullyConnected> newNode = std::make_shared<AutomatonNodeFullyConnected>(nodeName, nodes);
		(*nodes)[nodeName] = newNode;
	}
	return (*nodes)[nodeName];
}

int AutomatonFullyConnected::loadAllCallables()
{
	for(auto node: *nodes)
	{
		_callable call = (_callable)GetProcAddress(handle, node.first.c_str());
		if (!call)
		{
			std::cerr << "ERROR: Could not locate the function " << node.first << ". Revert to using the default callable" << std::endl;
			continue;
		}
		node.second->setCallable(call);
	}

	return 0;
}
