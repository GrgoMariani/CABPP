#ifndef FRAMEWORK_AUTOMATON_AUTOMATONNODEFULLYCONNECTED_H_
#define FRAMEWORK_AUTOMATON_AUTOMATONNODEFULLYCONNECTED_H_

#include <map>
#include <string>
#include <memory>

#include "AutomatonNodeBase.h"

#include "Callable.h"

class AutomatonNodeFullyConnected : public AutomatonNodeBase {
	std::shared_ptr<std::map<std::string, std::shared_ptr<AutomatonNodeFullyConnected>>> edges; // <command, Node>
public:
	AutomatonNodeFullyConnected(std::string name, std::shared_ptr<std::map<std::string, std::shared_ptr<AutomatonNodeFullyConnected>>> edges);
	~AutomatonNodeFullyConnected();
	void connectCommand(std::string command, std::shared_ptr<AutomatonNodeBase> nextNode);
public:
	std::shared_ptr<AutomatonNodeBase> inputCommand(std::string command, std::string argument);
};


#endif /* FRAMEWORK_AUTOMATON_AUTOMATONNODEFULLYCONNECTED_H_ */
