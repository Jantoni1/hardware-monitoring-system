#include "stdafx.h"
#include "socket_controller.h"


//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

const int default_bufflen = 24;

socket_controller::socket_controller(const std::string& ip, const std::string& port)
	: authorization_done(false)
	, ConnectSocket(INVALID_SOCKET)
	, ip_(ip)
	, port_(port)
	, result_address_(nullptr)
{
	set_tcp_hints_values();

}


socket_controller::~socket_controller()
{
}



int socket_controller::connect1()
{
	struct addrinfo *result = NULL,
		*ptr = NULL;


	////// DO WYRZUCENIA
	char *sendbuf = "this is a test";
	char recvbuf[default_bufflen];
	int iResult;
	int recvbuflen = default_bufflen;
	////// DO WYRZUCENIA

	initialize_winsock();
	resolve_ip_address_and_port();



	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, static_cast<int>(strlen(sendbuf)), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		clean_up(ConnectSocket);
		return 1;
	}

	try
	{
		while(!authorization_done)
		{
			// Receive until the peer closes the connection
			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			check_receive_result(iResult);
		}
	}
	catch(std::exception& e)
	{
		closesocket(ConnectSocket);
		shut_down_socket(ConnectSocket, SD_BOTH);
	}


	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	
	iResult = shutdown(ConnectSocket, SD_BOTH);
	if(check_socket_operation_error(ConnectSocket, iResult))
	{
		throw std::exception(); //TODO zmien na inny wyjatek
	}


	clean_up(ConnectSocket);
	return 0;
}

void socket_controller::check_receive_result(int result) const
{
	if (result == 0 && !authorization_done || result < 0)
		throw std::exception(); //TODO zmieñ na custom exception

	// TODO zmien na wyjatek tutaj bardziej w sumie chociaz sam nie wiem
	// gowniane to sterowanie
}

void socket_controller::shut_down_socket(SOCKET socket, int mode)
{
	const int result = shutdown(socket, mode);
	if(check_socket_operation_error(socket, result))
	{
		throw std::exception(); //TODO zmien na lepszy wyj¹tek
	}
}

bool socket_controller::check_socket_operation_error(SOCKET socket, int result)
{
	if(result == SOCKET_ERROR)
	{
		clean_up(socket);
		return true;
	}
	return false;
}

void socket_controller::clean_up(SOCKET socket)
{
	closesocket(socket);
	WSACleanup();
}

void socket_controller::set_tcp_hints_values()
{
	// Fill memory with zeros, starts on hint's address and fills next siezof(hints) with zeros
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}

void socket_controller::initialize_winsock()
{
	// Initialize Winsock
	// Retreieves implementation data from underlying windows OS
	const auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		throw std::runtime_error(std::string("WSAStartup failed with error: ") + std::to_string(result));
	}
}

void socket_controller::resolve_ip_address_and_port()
{
	// Resolve the server address and port
	const auto result = getaddrinfo(ip_.c_str(), port_.c_str(), &hints, &result_address_);
	if (result != 0) {
		throw std::runtime_error(std::string("getaddrinfo failed with error: ") + std::to_string(result));
		WSACleanup();
		return 1;
	}
}
