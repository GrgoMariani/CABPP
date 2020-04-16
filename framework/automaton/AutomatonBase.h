#ifndef FRAMEWORK_AUTOMATON_AUTOMATONBASE_H_
#define FRAMEWORK_AUTOMATON_AUTOMATONBASE_H_

#include <map>
#include <string>
#include <memory>
#include <windows.h>

#include "AutomatonNodeDirected.h"

class AutomatonBase {
protected:
	std::string name;
	std::string modulePath;

	HINSTANCE handle;

	std::shared_ptr<AutomatonNodeBase> current_state;
public:
	AutomatonBase(std::string name, std::string modulePath);
	virtual ~AutomatonBase() = 0;

	std::string getName() const;

	int inputCommand(std::string command, std::string arguments);
	int loadBinary();

	virtual std::shared_ptr<AutomatonNodeBase> getNode(std::string nodeName) = 0; // creates if non existing
	virtual int loadAllCallables() = 0;

};


#endif//FRAMEWORK_AUTOMATON_AUTOMATONBASE_H_
