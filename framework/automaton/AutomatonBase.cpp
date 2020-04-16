#include <iostream>

#include "AutomatonBase.h"
#include "Callable.h"

AutomatonBase::AutomatonBase(std::string name, std::string modulePath)
	: name(name), modulePath(modulePath), handle()
{

}

AutomatonBase::~AutomatonBase()
{

}

std::string AutomatonBase::getName() const
{
	return name;
}


int AutomatonBase::inputCommand(std::string command, std::string argument)
{
	current_state = std::static_pointer_cast<AutomatonNodeBase>(current_state->inputCommand(command, argument));
	return 0;
}

int AutomatonBase::loadBinary()
{
	handle =  LoadLibrary(modulePath.c_str());
	if (!handle)
	{
	    std::cerr << "could not load the dynamic library " << modulePath << std::endl;
	    return EXIT_FAILURE;
	}
	return 0;
}
