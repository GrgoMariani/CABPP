#ifndef FRAMEWORK_LOGGER_LOGGER_H_
#define FRAMEWORK_LOGGER_LOGGER_H_

#include <string>
#include <queue>
#include <utility>
#include <mutex>
#include <thread>
#include <memory>

enum LOG_LEVELS
{
	LOGLVL_TRACE	= 0,
	LOGLVL_DEBUG	= 10,
	LOGLVL_INFO		= 20,
	LOGLVL_SUCCESS	= 30,
	LOGLVL_WARNING	= 40,
	LOGLVL_ERROR	= 50,
	LOGLVL_CRITICAL	= 60
};

class ShitLogger : public std::enable_shared_from_this<ShitLogger> {

	std::recursive_mutex mtx;
	std::queue<std::pair<int, std::string>> log_queue;
	int loglvl;
	std::shared_ptr<std::thread> loggerthread;
public:
	ShitLogger();
	void setLogLevel(int newLogLvl);
	void pushToLog(int msg_lvl, std::string message);
private:
	void loopLatestMessages();
public:
	void runLoggerThread();
	void joinLoggerThread();
};


#endif /* FRAMEWORK_LOGGER_LOGGER_H_ */
