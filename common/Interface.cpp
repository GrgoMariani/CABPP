#include "SharedFunctions.h"

#include <string>
#include <windows.h>

extern "C" typedef int (*call_command)(std::string, std::string);

int CALL_COMMAND(std::string command, std::string argument)
{
	static call_command pExeFunc = nullptr;
	if(pExeFunc)
	{
		return pExeFunc(command, argument);
	}

	HMODULE hFramework = GetModuleHandle(NULL);
	pExeFunc = (call_command) GetProcAddress(hFramework, "CALL_COMMAND");
	if(pExeFunc)
	{
		return pExeFunc(command, argument);
	}
	return -1;
}

extern "C" typedef int (*log_message)(int, std::string);

int LOG_MESSAGE(int loglvl, std::string message)
{
	static log_message pExeFunc = nullptr;
	if(pExeFunc)
	{
		return pExeFunc(loglvl, message);
	}

	HMODULE hFramework = GetModuleHandle(NULL);
	pExeFunc = (log_message) GetProcAddress(hFramework, "LOG_MESSAGE");
	if(pExeFunc)
	{
		return pExeFunc(loglvl, message);
	}
	return -1;
}
