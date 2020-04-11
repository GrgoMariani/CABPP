#include <string>

#include "../tinyxml2/tinyxml2.h"
#include "../common/SharedFunctions.h"

#include <sstream>
#include <map>


static int num_connections = 0;
static std::map<std::string, int> connections;


STATE(_start_)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE _start_ ["+argument+"]");
	return 0;
}

STATE(socket_connected)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE socket_connected ["+argument+"]");

	std::string ip_port; 		// get from argument

	tinyxml2::XMLDocument doc;
	doc.Parse(argument.c_str(), argument.length());
	tinyxml2::XMLElement * pData = doc.FirstChildElement("data");
	const char * ipporttext = pData->FirstChildElement("ipport")->GetText();
	ip_port = std::string(ipporttext);
	std::string message;
	connections[ip_port] = ++num_connections;
	{
		std::stringstream s;
		s << "Just connected " << ipporttext << " as connection #" << connections[ip_port];
		message = s.str();
	}

	LOG_MESSAGE(LOGLVL_SUCCESS, message);
	return 0;
}

STATE(socket_disconnected)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE disconnected ["+argument+"]");
	std::string ip_port; 		// get from argument

	tinyxml2::XMLDocument doc;
	doc.Parse(argument.c_str(), argument.length());
	tinyxml2::XMLElement * pData = doc.FirstChildElement("data");
	const char * ipporttext = pData->FirstChildElement("ipport")->GetText();
	ip_port = std::string(ipporttext);

	connections[ip_port] = ++num_connections;
	std::stringstream s;
	std::string message;
	s << "Just disconnected " << ip_port << " who joined as connection #" << connections[ip_port];
	message = s.str();
	LOG_MESSAGE(LOGLVL_SUCCESS, message);

	return 0;
}

STATE(parse_data)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE parse_data ["+argument+"]");
	std::string ip_port; 		// get from argument
	std::string msg_recvd; 	// get from argument

	tinyxml2::XMLDocument doc;

	doc.Parse(argument.c_str(), argument.length());
	tinyxml2::XMLElement * pData = doc.FirstChildElement("data");
	const char * ipporttext = pData->FirstChildElement("ipport")->GetText();
	const char * msgtext = pData->FirstChildElement("msg")->GetText();
	ip_port = std::string(ipporttext);
	msg_recvd = std::string(msgtext);

	std::stringstream s;
	std::string message;
	s << "<data><ipport>" << ip_port << "</ipport><msg>";
	s << "Please STOP sending this : " << std::endl << msg_recvd << std::endl << std::endl << std::endl << ip_port << " who joined as connection " << connections[ip_port];
	s << "</msg></data>";
	message = s.str();
	LOG_MESSAGE(LOGLVL_SUCCESS, "Sending message"+std::string(message));
	CALL_COMMAND("add_message_to_be_sent", std::string(message));
	return 0;
}
