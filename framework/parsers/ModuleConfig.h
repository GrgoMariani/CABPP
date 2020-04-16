#ifndef FRAMEWORK_PARSERS_MODULECONFIG_H_
#define FRAMEWORK_PARSERS_MODULECONFIG_H_

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "../automaton/AutomatonBase.h"
#include "Edge.h"


class ModuleConfig {
	std::string name;

	std::string modulePath;
	std::string graphPath;

	std::map<std::string, std::string> nodes;	//use std::map string string
	std::vector<Edge> edges;
public:
	ModuleConfig(std::string name, std::string modulePath, std::string graphPath);

	std::string getName();
	std::string getModulePath();

	int readGraph();

	std::shared_ptr<AutomatonBase> createAutomatonTopology();
};



#endif//FRAMEWORK_PARSERS_MODULECONFIG_H_
