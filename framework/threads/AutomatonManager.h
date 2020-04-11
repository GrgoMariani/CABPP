#ifndef FRAMEWORK_THREADS_AUTOMATONMANAGER_H_
#define FRAMEWORK_THREADS_AUTOMATONMANAGER_H_

#include <string>
#include <vector>
#include <memory>

#include "AutomatonThread.h"
#include "../logger/Logger.h"

class AutomatonManager : public std::enable_shared_from_this<AutomatonManager>
{
	std::vector<std::shared_ptr<AutomatonThread>> automatonThreads;
	std::vector<std::shared_ptr<std::thread>> runningThreads;
	std::shared_ptr<ShitLogger> logger;
public:
	AutomatonManager(std::map<std::string, std::shared_ptr<Automaton>>& automatons);
public:
	void setLogger(std::shared_ptr<ShitLogger> newLogger);

	void connectInterfaceCommands();

	int runAllThreads();
	int inputCommand(std::string command, std::string argument);
	int joinAllThreads();
};



#endif /* FRAMEWORK_THREADS_AUTOMATONMANAGER_H_ */
