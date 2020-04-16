#ifndef FRAMEWORK_AUTOMATON_AUTOMATONFULLYCONNECTED_H_
#define FRAMEWORK_AUTOMATON_AUTOMATONFULLYCONNECTED_H_

#include <string>
#include <memory>
#include <map>

#include "AutomatonBase.h"
#include "AutomatonNodeFullyConnected.h"

class AutomatonFullyConnected : public AutomatonBase {

	std::shared_ptr<std::map<std::string, std::shared_ptr<AutomatonNodeFullyConnected>>> nodes;
public:
	AutomatonFullyConnected(std::string name, std::string modulePath);
	~AutomatonFullyConnected();

	std::shared_ptr<AutomatonNodeBase> getNode(std::string nodeName);
	int loadAllCallables();

};

#endif /* FRAMEWORK_AUTOMATON_AUTOMATONFULLYCONNECTED_H_ */
