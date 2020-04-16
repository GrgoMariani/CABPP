#include "../../tinyxml2/tinyxml2.h"
#include "FrameworkConfig.h"


FrameworkConfig::FrameworkConfig(std::string configPath) : configPath(configPath), modules(), automatons()
{

}

int FrameworkConfig::readConfigFromFile()
{
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLError eResult = xmlDoc.LoadFile(configPath.c_str());
	if(eResult != tinyxml2::XML_SUCCESS)
		return 1;
	tinyxml2::XMLNode * pFramework = xmlDoc.FirstChildElement("framework");
	if (pFramework == nullptr)
		return 1;
	tinyxml2::XMLElement * pAutomata = pFramework->FirstChildElement("automata");
	while(pAutomata != nullptr)
	{
		const char * name = nullptr;
		const char * modulePath = nullptr;
		const char * graphPath = nullptr;
		name = pAutomata->Attribute("name");
		modulePath = pAutomata->Attribute("binary");
		graphPath = pAutomata->Attribute("graph");
		if(modulePath == nullptr || graphPath == nullptr)
			continue;
		modules.push_back(ModuleConfig(std::string(name), std::string(modulePath), std::string(graphPath)));
		pAutomata = pAutomata->NextSiblingElement("automata");
	}
	return 0;
}


int FrameworkConfig::readAllModulesGraphs()
{
	for(auto& module : modules)
		module.readGraph();
	return 0;
}

void FrameworkConfig::createAllModulesTopology()
{
	for(auto& module : modules)
	{
		std::string name = module.getName();
		automatons[name] = module.createAutomatonTopology();
	}
}

void FrameworkConfig::loadAllModules()
{
	for(auto& automaton : automatons)
	{
		if(automaton.second->loadBinary() == 0)
		{
			automaton.second->loadAllCallables();
		}
	}
}

std::shared_ptr<AutomatonManager> FrameworkConfig::createAutomatonManager()
{
	return std::make_shared<AutomatonManager>(automatons);
}
