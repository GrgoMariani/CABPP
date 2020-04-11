#ifndef FRAMEWORK_PARSERS_FRAMEWORKCONFIG_H_
#define FRAMEWORK_PARSERS_FRAMEWORKCONFIG_H_

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Edge.h"
#include "../automaton/Automaton.h"
#include "../../tinyxml2/tinyxml2.h"
#include "../threads/AutomatonManager.h"

#include "ModuleConfig.h"


class FrameworkConfig {
	std::string configPath;
	std::vector<ModuleConfig> modules;
	std::map<std::string, std::shared_ptr<Automaton>> automatons; // <name, automaton>
public:
	FrameworkConfig(std::string configPath);

	int readConfigFromFile();
	int readAllModulesGraphs();
	void createAllModulesTopology();
	void loadAllModules();

	std::shared_ptr<AutomatonManager> createAutomatonManager();
};

#endif//FRAMEWORK_PARSERS_FRAMEWORKCONFIG_H_
