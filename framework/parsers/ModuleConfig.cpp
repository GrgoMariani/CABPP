#include "ModuleConfig.h"

#include "../../tinyxml2/tinyxml2.h"

#include "../automaton/AutomatonDirected.h"
#include "../automaton/AutomatonFullyConnected.h"

ModuleConfig::ModuleConfig(std::string name, std::string modulePath, std::string graphPath, std::string graphType)
	: name(name), modulePath(modulePath), graphPath(graphPath), graphType(graphType), nodes(), edges()
{

}

std::string ModuleConfig::getName()
{
	return name;
}

std::string ModuleConfig::getModulePath()
{
	return modulePath;
}


int ModuleConfig::readGraph()
{
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLError eResult = xmlDoc.LoadFile(("xmlsettings/" + graphPath).c_str());
	if(eResult != tinyxml2::XML_SUCCESS)
		return 1;
	tinyxml2::XMLElement * pGraphML = xmlDoc.FirstChildElement("graphml");
	if (pGraphML == nullptr)
		return 1;
	tinyxml2::XMLElement * pGraph = pGraphML->FirstChildElement("graph");
	if (pGraph == nullptr)
		return 1;
	tinyxml2::XMLElement * pNode = pGraph->FirstChildElement("node");
	while(pNode != nullptr)
	{
		const char * id = nullptr;
		id = pNode->Attribute("id");
		if(id != nullptr)
		{
			std::string node_id(id);
			tinyxml2::XMLElement * pData = pNode->FirstChildElement("data");
			while(pData != nullptr)
			{
				if(pData->Attribute("key", "d6") != nullptr)
				{
					tinyxml2::XMLElement * pShapeNode = pData->FirstChildElement("y:ShapeNode");
					if(pShapeNode != nullptr)
					{
						tinyxml2::XMLElement * pNodeLabel = pShapeNode->FirstChildElement("y:NodeLabel");
						const char * label = nullptr;
						label = pNodeLabel->GetText();
						if(label != nullptr)
						{
							std::string node_label(label);
							nodes[node_id] = node_label;
						}
					}
				}
				pData = pData->NextSiblingElement("data");
			}
		}
		pNode = pNode->NextSiblingElement("node");
	}
	tinyxml2::XMLElement * pEdge = pGraph->FirstChildElement("edge");
	while(pEdge != nullptr)
	{
		Edge e;
		const char * source = nullptr;
		const char * target = nullptr;
		source = pEdge->Attribute("source");
		target = pEdge->Attribute("target");
		if(source != nullptr && target != nullptr)
		{
			e.source = std::string(source);
			e.target = std::string(target);
			tinyxml2::XMLElement * pData = pEdge->FirstChildElement("data");
			while(pData != nullptr)
			{
				if(pData->Attribute("key", "d10") != nullptr)
				{
					tinyxml2::XMLElement * pLineEdge = pData->FirstChildElement();//
					if(pLineEdge != nullptr)
					{
						tinyxml2::XMLElement * pEdgeLabel = pLineEdge->FirstChildElement("y:EdgeLabel");
						const char * label = nullptr;
						label = pEdgeLabel->GetText();
						if(label != nullptr)
						{
							e.command = std::string(label);
							this->edges.push_back(e);
						}
					}
				}
				pData = pData->NextSiblingElement("data");
			}
		}
		pEdge = pEdge->NextSiblingElement("edge");
	}
	return 0;
}

std::shared_ptr<AutomatonBase> ModuleConfig::createAutomatonTopology()
{
	if(graphType.compare("directional")==0)
	{
		std::shared_ptr<AutomatonDirected> newDirecteddAutomaton = std::make_shared<AutomatonDirected>(name, modulePath);
		for(auto& node: nodes)
		{
			newDirecteddAutomaton->getNode(node.second);	// this is enough to just add them
		}
		for(auto& edge: edges)
		{
			std::string ssource = nodes[edge.source];
			std::string starget = nodes[edge.target];
			auto source = newDirecteddAutomaton->getNode(ssource);
			auto target = newDirecteddAutomaton->getNode(starget);
			source->connectCommand(edge.command, target);
		}
		return std::static_pointer_cast<AutomatonBase>(newDirecteddAutomaton);
	}
	if(graphType.compare("fully_connected")==0)
	{
		std::shared_ptr<AutomatonFullyConnected> newFullyConnectedAutomaton = std::make_shared<AutomatonFullyConnected>(name, modulePath);
		for(auto& node: nodes)
		{
			auto new_node = newFullyConnectedAutomaton->getNode(node.second);
			new_node->connectCommand(node.second, new_node);
		}
		return std::static_pointer_cast<AutomatonBase>(newFullyConnectedAutomaton);
	}
	return nullptr;
}
