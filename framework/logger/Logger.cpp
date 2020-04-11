#include <iostream>
#include <chrono>

#include "Logger.h"

ShitLogger::ShitLogger() : mtx(), log_queue(), loglvl(LOGLVL_INFO), loggerthread(nullptr)
{

}

void ShitLogger::setLogLevel(int newLogLvl)
{
	mtx.lock();
	loglvl = newLogLvl;
	mtx.unlock();
}

void ShitLogger::pushToLog(int msg_lvl, std::string message)
{
	if(msg_lvl < loglvl)
		return;
	std::pair<int, std::string> newLogMessage(msg_lvl, message);
	mtx.lock();
	log_queue.emplace(newLogMessage);
	mtx.unlock();
}

void ShitLogger::loopLatestMessages()
{
	while(1)
	{
		if(!log_queue.empty())
		{
			mtx.lock();
			std::pair<int, std::string> nextLogMessage = log_queue.front();
			log_queue.pop();
			mtx.unlock();
			{
				std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				std::string time(32, '\0');
				std::strftime(&time[0], time.size(), "%H:%M:%S", std::localtime(&now));
				std::cout << "[" << nextLogMessage.first << " @ " << time << "] " << nextLogMessage.second  << std::endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void ShitLogger::runLoggerThread()
{
	loggerthread = std::make_shared<std::thread>(&ShitLogger::loopLatestMessages, shared_from_this());
}

void ShitLogger::joinLoggerThread()
{
	loggerthread->join();
	loggerthread = nullptr;
}
