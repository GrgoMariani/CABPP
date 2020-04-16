#ifndef FRAMEWORK_AUTOMATON_AUTOMATONNODEDIRECTED_H_
#define FRAMEWORK_AUTOMATON_AUTOMATONNODEDIRECTED_H_

#include <map>
#include <string>
#include <memory>

#include "AutomatonNodeBase.h"

#include "Callable.h"

class AutomatonNodeDirected : public AutomatonNodeBase {
	std::map<std::string, std::shared_ptr<AutomatonNodeDirected>> edges; // <command, Node>
public:
	AutomatonNodeDirected(std::string name);
	~AutomatonNodeDirected();
	void connectCommand(std::string command, std::shared_ptr<AutomatonNodeBase> nextNode);
public:
	std::shared_ptr<AutomatonNodeBase> inputCommand(std::string command, std::string argument); // calls callable , returns the next node or nullptr
};



#endif /* FRAMEWORK_AUTOMATON_AUTOMATONNODEDIRECTED_H_ */
