#ifndef FRAMEWORK_AUTOMATON_CALLABLE_H_
#define FRAMEWORK_AUTOMATON_CALLABLE_H_

#include <string>

extern "C" typedef int (__stdcall *_callable)(std::string s);

int default_callable(std::string s);

#endif /* FRAMEWORK_AUTOMATON_CALLABLE_H_ */
