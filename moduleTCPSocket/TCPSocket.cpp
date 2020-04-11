#include "../tinyxml2/tinyxml2.h"
#include "../common/SharedFunctions.h"

#include <winsock2.h>
#include <string>
#include <cstring>
#include <ctime>
#include <sstream>

#include <iostream>

const int BUFFER_SIZE = 2048;
const int PORT = 52000;
const int MAX_SOCKETS = 128;
const int EMPTY = 0;
const int LISTEN  = 1;
const int RECEIVE = 2;
const int IDLE = 3;
const int SEND = 4;


struct SocketData
{
	SOCKET id;			// Handle
	int	recv;			// Is receiving
	int	send;			// Is sending
	char buffer[BUFFER_SIZE];
	std::string ip_port;
	std::string msg_to_send;
};


SOCKET listenSocket;
struct SocketData sockets[MAX_SOCKETS] = {0};
int socketsCount = 0;
fd_set waitRecv;
fd_set waitSend;
int nfd;

/* HELPING FUNCTIONS */
bool addSocket(SOCKET id, int what, std::string ip_port);
void removeSocket(int index);
void acceptConnection(int index);
void receiveMessage(int index);
void sendMessage(int index);



STATE(_start_)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE _start_ ["+argument+"]");
	return 0;
}

STATE(error)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE error ["+argument+"]");
	LOG_MESSAGE(LOGLVL_CRITICAL, "TCP_Socket module ran into an error : "+argument);
	return 0;
}

STATE(startup)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE startup ["+argument+"]");
	WSAData wsaData;

	if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR)
	{
		CALL_COMMAND("error", "<data>Error at WSAStartup</data>");
		return 1;
	}

	CALL_COMMAND("create_listen_socket", "<data></data>");
	return 0;
}


STATE(listen_socket_creation)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE listen_socket_creation ["+argument+"]");
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (listenSocket == INVALID_SOCKET)
	{
		CALL_COMMAND("error", "<data>Error at socket</data>");
		WSACleanup();
		return 1;
	}

	CALL_COMMAND("bind", "<data></data>");
	return 0;
}


STATE(binding)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE binding ["+argument+"]");
	sockaddr_in serverService;
	serverService.sin_family 		= AF_INET;
	serverService.sin_addr.s_addr 	= INADDR_ANY;
	serverService.sin_port 			= htons(PORT);
	if (bind(listenSocket, (SOCKADDR *) &serverService, sizeof(serverService)) == SOCKET_ERROR)
	{
		CALL_COMMAND("error", "<data>Error at bind</data>");
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	CALL_COMMAND("create_listen_port", "<data></data>");
	return 0;
}


STATE(creating_listening_port)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE creating_listening_port ["+argument+"]");
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		CALL_COMMAND("error", "<data>Error at listen</data>");
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	addSocket(listenSocket, LISTEN, "0.0.0.0:52000");
	CALL_COMMAND("handle_connections", "<data></data>");
	return 0;
}

STATE(handling_connections)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE handling_connections ["+argument+"]");
	FD_ZERO(&waitRecv);
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if ((sockets[i].recv == LISTEN) || (sockets[i].recv == RECEIVE))
			FD_SET(sockets[i].id, &waitRecv);
	}

	FD_ZERO(&waitSend);
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (sockets[i].send == SEND)
			FD_SET(sockets[i].id, &waitSend);
	}
	timeval timeout{0, 0};

	nfd = select(0, &waitRecv, &waitSend, NULL, &timeout);
	if (nfd == SOCKET_ERROR)
	{
		CALL_COMMAND("error", "<data>Error at select</data>");
		WSACleanup();
		return 1;
	}
	for (int i = 0; i < MAX_SOCKETS && nfd > 0; i++)
	{
		if (FD_ISSET(sockets[i].id, &waitRecv))
		{
			nfd--;
			switch (sockets[i].recv)
			{
			case LISTEN:
				acceptConnection(i);
				break;
			case RECEIVE:
				receiveMessage(i);
				break;
			}
		}
	}

	for (int i = 0; i < MAX_SOCKETS && nfd > 0; i++)
	{
		if (FD_ISSET(sockets[i].id, &waitSend))
		{
			nfd--;
			switch (sockets[i].send)
			{
			case SEND:
				sendMessage(i);
				break;
			}
		}
	}
	CALL_COMMAND("handle_connections", "<data></data>");
	return 0;
}


STATE(close_listening_socket)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE close_listening_socket ["+argument+"]");
	closesocket(listenSocket);
	WSACleanup();
	return 0;
}

STATE(add_message_for_sending)
{
	LOG_MESSAGE(LOGLVL_TRACE, "STATE add_message_for_sending ["+argument+"]");
	std::string ip_port; 		// get from argument
	std::string msg_to_send; 	// get from argument

	tinyxml2::XMLDocument doc;
	doc.Parse(argument.c_str(), argument.length());
	tinyxml2::XMLElement * pData = doc.FirstChildElement("data");
	ip_port 	= std::string(pData->FirstChildElement("ipport")->GetText());
	msg_to_send = std::string(pData->FirstChildElement("msg")->GetText());

	for(int i=0; i<socketsCount; i++)
	{
		SocketData& socket = sockets[i];
		if(ip_port.compare(socket.ip_port) == 0)
		{
			socket.send = SEND;
			socket.msg_to_send = msg_to_send;
			break;
		}
	}
	CALL_COMMAND("handle_connections", "<data></data>");

	return 0;
}




/* ***************** */
/* HELPING FUNCTIONS */
/* ***************** */
/* ***************** */
/* ***************** */

bool addSocket(SOCKET id, int what, std::string ip_port)
{
	for (int i = 0; i < MAX_SOCKETS; i++)
	{
		if (sockets[i].recv == EMPTY)
		{
			sockets[i].id 	= id;
			sockets[i].recv = what;
			sockets[i].send = IDLE;
			sockets[i].ip_port = ip_port;
			socketsCount++;
			return true;
		}
	}
	return false;
}

void removeSocket(int index)
{
	sockets[index].recv = EMPTY;
	sockets[index].send = EMPTY;
	socketsCount--;
}

void acceptConnection(int index)
{
	SOCKET id = sockets[index].id;
	struct sockaddr_in from;		// Address of sending partner
	int fromLen = sizeof(from);

	SOCKET msgSocket = accept(id, (struct sockaddr *)&from, &fromLen);
	if (INVALID_SOCKET == msgSocket)
	{
		LOG_MESSAGE(LOGLVL_WARNING, "Error while accepting socket");
		return;
	}

	std::string ip_port;
	{
		std::stringstream s;
		s << inet_ntoa(from.sin_addr) << ":" << ntohs(from.sin_port);
		ip_port = s.str();
	}
	LOG_MESSAGE(LOGLVL_SUCCESS, "Connected from "+ip_port);
	CALL_COMMAND("socket_connection_success", "<data><ipport>"+ip_port+"</ipport></data>");

	unsigned long NON_BLOCKING = 1;
	if (ioctlsocket(msgSocket, FIONBIO, &NON_BLOCKING) != 0)
	{
		LOG_MESSAGE(LOGLVL_WARNING, "Error at ioctlsocket");
	}

	if (addSocket(msgSocket, RECEIVE, ip_port) == false)
	{
		LOG_MESSAGE(LOGLVL_WARNING, "Dropped socket because of too many connections");
		CALL_COMMAND("socket_disconnected", "<data><ipport>"+ip_port+"</ipport></data>");
		closesocket(id);
	}
	return;
}

void receiveMessage(int index)
{
	SOCKET msgSocket = sockets[index].id;

	int bytesRecv = recv(msgSocket, sockets[index].buffer, BUFFER_SIZE, 0);

	if (bytesRecv == SOCKET_ERROR)
	{
		LOG_MESSAGE(LOGLVL_WARNING, "Error while receiving data from "+sockets[index].ip_port+" . Closing socket");
		CALL_COMMAND("socket_disconnected", "<data><ipport>"+sockets[index].ip_port+"</ipport></data>");
		closesocket(msgSocket);
		removeSocket(index);
		return;
	}
	if (bytesRecv == 0)
	{
		LOG_MESSAGE(LOGLVL_WARNING, "Closing socket "+sockets[index].ip_port);
		CALL_COMMAND("socket_disconnected", "<data><ipport>"+sockets[index].ip_port+"</ipport></data>");
		closesocket(msgSocket);
		removeSocket(index);
		return;
	}
	else
	{
		std::string msg_recvd(sockets[index].buffer);
		LOG_MESSAGE(LOGLVL_SUCCESS, sockets[index].ip_port + " - received: "+msg_recvd);
		std::string msg_to_send("<data><ipport>"+sockets[index].ip_port+"</ipport><msg>"+msg_recvd+"</msg></data>");
		CALL_COMMAND("data_received", msg_to_send);
	}
}

void sendMessage(int index)
{
	int bytesSent = 0;
	SocketData& socket = sockets[index];
	std::string msg_to_be_sent = socket.msg_to_send;

	bytesSent = send(socket.id, msg_to_be_sent.c_str(), msg_to_be_sent.length(), 0);
	if (bytesSent == SOCKET_ERROR)
	{
		LOG_MESSAGE(LOGLVL_WARNING, "Error while sending "+ socket.ip_port);
		return;
	}
	LOG_MESSAGE(LOGLVL_SUCCESS, "Sent "+msg_to_be_sent+" to "+ socket.ip_port);
	sockets[index].send = IDLE;
	LOG_MESSAGE(LOGLVL_INFO, "Closing socket");
	CALL_COMMAND("socket_disconnected", "<data><ipport>"+socket.ip_port+"</ipport></data>");
	closesocket(socket.id);
	removeSocket(index);
}
