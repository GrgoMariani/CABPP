#include <iostream>

#include "AutomatonDirected.h"

AutomatonDirected::AutomatonDirected(std::string name, std::string modulePath) : AutomatonBase(name, modulePath)
{
	current_state = getNode("_start_");
}

AutomatonDirected::~AutomatonDirected()
{

}

std::shared_ptr<AutomatonNodeBase> AutomatonDirected::getNode(std::string nodeName)
{
	if( nodes.count(nodeName)==0 )
	{
		std::shared_ptr<AutomatonNodeDirected> newNode = std::make_shared<AutomatonNodeDirected>(nodeName);
		nodes[nodeName] = newNode;
	}
	return nodes[nodeName];
}

int AutomatonDirected::loadAllCallables()
{
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
