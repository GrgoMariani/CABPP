#ifndef FRAMEWORK_AUTOMATON_AUTOMATONNODE_H_
#define FRAMEWORK_AUTOMATON_AUTOMATONNODE_H_

#include <map>
#include <string>
#include <memory>

#include "Callable.h"

class AutomatonNode : public std::enable_shared_from_this<AutomatonNode> {
	std::string 	name;
	_callable		callable;
	std::map<std::string, std::shared_ptr<AutomatonNode>> edges; // <command, Node>
public:
	AutomatonNode(std::string name);
	std::string getName() const;
	void connectCommand(std::string command, std::shared_ptr<AutomatonNode> nextNode);
	int setCallable(_callable newCallable);
private:
	int callCallable(std::string argument);
public:
	std::shared_ptr<AutomatonNode> inputCommand(std::string command, std::string argument); // calls callable , returns the next node or nullptr
};



#endif /* FRAMEWORK_AUTOMATON_AUTOMATONNODE_H_ */
