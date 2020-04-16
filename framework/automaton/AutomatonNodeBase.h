#ifndef FRAMEWORK_AUTOMATON_AUTOMATONNODEBASE_H_
#define FRAMEWORK_AUTOMATON_AUTOMATONNODEBASE_H_

#include <map>
#include <string>
#include <memory>

#include "Callable.h"

class AutomatonNodeBase : public std::enable_shared_from_this<AutomatonNodeBase>
{
	std::string 	name;
	_callable		callable;
public:
	AutomatonNodeBase(std::string name);
	virtual ~AutomatonNodeBase() = 0;
	std::string getName() const;
	virtual void connectCommand(std::string command, std::shared_ptr<AutomatonNodeBase> nextNode) = 0;
	int setCallable(_callable newCallable);
protected:
	int callCallable(std::string argument);
public:
	virtual std::shared_ptr<AutomatonNodeBase> inputCommand(std::string command, std::string argument) = 0; // calls callable , returns the next node or nullptr

};


#endif /* FRAMEWORK_AUTOMATON_AUTOMATONNODEBASE_H_ */
