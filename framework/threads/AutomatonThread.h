#ifndef FRAMEWORK_THREADS_AUTOMATONTHREAD_H_
#define FRAMEWORK_THREADS_AUTOMATONTHREAD_H_

#include <string>
#include <queue>
#include <utility>
#include <memory>
#include <thread>
#include <mutex>

#include "../automaton/Automaton.h"


class AutomatonThread : public std::enable_shared_from_this<AutomatonThread>{
	enum State
	{
		NOT_INITIALIZED, WAITING, PROCESSING, STOPPING, STOPPED
	};

	std::recursive_mutex mtx;
	std::shared_ptr<Automaton> automaton;
	std::string name;

	int state;
	std::queue<std::pair<std::string, std::string>> cmd_queue;
public:
	AutomatonThread(std::shared_ptr<Automaton> newAutomaton);

	int loop(); // the only function that will be in another thread
	int inputCommand(std::string cmd, std::string argument);
	int stop();

};




#endif /* FRAMEWORK_THREADS_AUTOMATONTHREAD_H_ */
