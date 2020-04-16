#include "AutomatonThread.h"
#include <chrono>

AutomatonThread::AutomatonThread(std::shared_ptr<AutomatonBase> newAutomaton)
	: mtx(), automaton(newAutomaton), name(automaton->getName()), state(NOT_INITIALIZED), cmd_queue()
{

}

int AutomatonThread::loop()
{
	if(state != NOT_INITIALIZED)
		return -1;				// already running
	std::pair<std::string, std::string> command_argument;
	while(1)
	{
		switch(state)
		{
		case NOT_INITIALIZED:
			mtx.lock();
			state = WAITING;
			mtx.unlock();
			break;
		case WAITING:
			if(!cmd_queue.empty())
			{
				mtx.lock();
				command_argument = cmd_queue.front();
				cmd_queue.pop();
				state = PROCESSING;
				mtx.unlock();
				automaton->inputCommand(command_argument.first, command_argument.second);
				mtx.lock();
				state = WAITING;
				mtx.unlock();
			}
			break;
		case PROCESSING:
			break;
		case STOPPING:
			mtx.lock();
			while(!cmd_queue.empty())
				cmd_queue.pop();
			mtx.unlock();
			// no break
		case STOPPED:
		default:
			goto LOOP_STOP;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	LOOP_STOP:
	state = STOPPED;
	return 0;
}

int AutomatonThread::inputCommand(std::string cmd, std::string argument)
{
	mtx.lock();
	if(state==WAITING || state==PROCESSING || state==NOT_INITIALIZED)
		cmd_queue.emplace(std::pair<std::string, std::string>(cmd, argument));
	mtx.unlock();
	return 0;
}

int AutomatonThread::stop()
{
	mtx.lock();
	state = STOPPING;
	mtx.unlock();
	return 0;
}
