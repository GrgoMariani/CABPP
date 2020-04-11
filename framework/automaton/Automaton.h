#ifndef FRAMEWORK_AUTOMATON_AUTOMATON_H_
#define FRAMEWORK_AUTOMATON_AUTOMATON_H_

#include <map>
#include <string>
#include <memory>
#include <windows.h>

#include "AutomatonNode.h"

class Automaton {
	std::string name;
	std::string modulePath;

	HINSTANCE handle;

	std::shared_ptr<AutomatonNode> current_state;

	std::map<std::string, std::shared_ptr<AutomatonNode>> nodes; // <nodeName, AutomatonNode>
public:
	Automaton(std::string name, std::string modulePath);

	std::string getName() const;

	std::shared_ptr<AutomatonNode> getNode(std::string nodeName); // creates if non existing

	int inputCommand(std::string command, std::string arguments);

	int loadBinary();

};


#endif//FRAMEWORK_AUTOMATON_AUTOMATON_H_
