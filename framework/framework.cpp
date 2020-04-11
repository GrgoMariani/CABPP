#include <windows.h>
#include <iostream>
#include <string>
#include "parsers/FrameworkConfig.h"
#include "logger/Logger.h"

int main(int argc, char *argv[])
{
	std::shared_ptr<ShitLogger> logger = std::make_shared<ShitLogger>();
	//logger->setLogLevel(LOGLVL_TRACE);
	logger->runLoggerThread();
	logger->pushToLog(LOGLVL_INFO, "Starting CABPP framework");

	FrameworkConfig fwConf("xmlsettings/framework.xml");

	fwConf.readConfigFromFile();
	fwConf.readAllModulesGraphs();
	fwConf.createAllModulesTopology();
	fwConf.loadAllModules();

	logger->pushToLog(LOGLVL_INFO, "Configuration and reading of modules complete");

	{
		logger->pushToLog(LOGLVL_INFO, "Setting up automation manager");
		auto mngr = fwConf.createAutomatonManager();
		mngr->setLogger(logger);
		mngr->connectInterfaceCommands();
		logger->pushToLog(LOGLVL_INFO, "Running all automatons in separate threads");
		mngr->runAllThreads();

		logger->pushToLog(LOGLVL_INFO, "Starting automatons:");

		mngr->inputCommand(std::string("_start_"), std::string(""));

		while(1)
		{
			logger->pushToLog(LOGLVL_INFO, "--------- Input command and argument");
			std::string command;
			std::string argument;
			std::cin>>command;
			std::cin>>argument;
			mngr->inputCommand(command, argument);
		}

		mngr->joinAllThreads();
		logger->joinLoggerThread();
	}

	return 0;
}

