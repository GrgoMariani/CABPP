#ifndef SHAREDFUNCTIONS_H_
#define SHAREDFUNCTIONS_H_

#include <string>

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

int CALL_COMMAND(std::string command, std::string argument);
int LOG_MESSAGE(int loglvl, std::string message);


#define STATE(statename) extern "C" int __declspec(dllexport) __stdcall statename(std::string argument)

#endif /* SHAREDFUNCTIONS_H_ */
