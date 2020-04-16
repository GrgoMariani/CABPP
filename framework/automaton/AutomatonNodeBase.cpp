#include "AutomatonNodeBase.h"

AutomatonNodeBase::AutomatonNodeBase(std::string name) : name(name), callable(default_callable)
{

}

AutomatonNodeBase::~AutomatonNodeBase()
{

}

std::string AutomatonNodeBase::getName() const
{
	return name;
}

int AutomatonNodeBase::setCallable(_callable newCallable)
{
	callable = newCallable;
	return 0;
}


int AutomatonNodeBase::callCallable(std::string argument)
{
	return callable(argument);
}
