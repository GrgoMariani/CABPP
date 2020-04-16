#ifndef FRAMEWORK_AUTOMATON_AUTOMATONDIRECTED_H_
#define FRAMEWORK_AUTOMATON_AUTOMATONDIRECTED_H_

#include "AutomatonBase.h"
#include "AutomatonNodeDirected.h"

class AutomatonDirected : public AutomatonBase {

	std::map<std::string, std::shared_ptr<AutomatonNodeDirected>> nodes; // <nodeName, AutomatonNode>
public:
	AutomatonDirected(std::string name, std::string modulePath);
	~AutomatonDirected();

	std::shared_ptr<AutomatonNodeBase> getNode(std::string nodeName);
	int loadAllCallables();

};


#endif /* FRAMEWORK_AUTOMATON_AUTOMATONDIRECTED_H_ */
