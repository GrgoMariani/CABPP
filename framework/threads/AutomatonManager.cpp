#include "AutomatonManager.h"

#include <iostream>

static std::shared_ptr<AutomatonManager> gAutomatonManager = nullptr;
static std::shared_ptr<ShitLogger> gLogger = nullptr;

extern "C" __declspec(dllexport) int CALL_COMMAND(std::string command, std::string argument)
{
	if(gAutomatonManager != nullptr)
		gAutomatonManager->inputCommand(command, argument);
	return 0;
}

extern "C" __declspec(dllexport) int LOG_MESSAGE(int loglvl, std::string message)
{
	if(gLogger != nullptr)
		gLogger->pushToLog(loglvl, message);
	return 0;
}

AutomatonManager::AutomatonManager(std::map<std::string, std::shared_ptr<AutomatonBase>>& automatons)
	: automatonThreads(), runningThreads(), logger(nullptr)
{
	for(auto& automaton : automatons)
	{
		std::shared_ptr<AutomatonThread> newThread = std::make_shared<AutomatonThread>(automaton.second);
		automatonThreads.push_back(newThread);
	}
}

void AutomatonManager::setLogger(std::shared_ptr<ShitLogger> newLogger)
{
	logger = newLogger;
}

void AutomatonManager::connectInterfaceCommands()
{
	gAutomatonManager = shared_from_this();
	gLogger = logger;
}

int AutomatonManager::runAllThreads()
{
	for(auto& single_automaton_thread : automatonThreads)
	{
		runningThreads.push_back(std::make_shared<std::thread>(&AutomatonThread::loop, single_automaton_thread));
	}
	return 0;
}

int AutomatonManager::inputCommand(std::string command, std::string argument)
{
	logger->pushToLog(LOGLVL_TRACE, "Calling ["+command+"] with argument ["+argument+"]");
	for(auto& thread : automatonThreads)
	{
		thread->inputCommand(command, argument);
	}
	return 0;
}

int AutomatonManager::joinAllThreads()
{
	for(auto& single_running_thread : runningThreads)
	{
		single_running_thread->join();
	}
	runningThreads.clear();
	return 0;
}
