#ifndef FRAMEWORK_PARSERS_EDGE_H_
#define FRAMEWORK_PARSERS_EDGE_H_

#include <string>

class Edge {
public:
	std::string source;
	std::string target;
	std::string command;

	Edge();
};


#endif//FRAMEWORK_PARSERS_EDGE_H_
