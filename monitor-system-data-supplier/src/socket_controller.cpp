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
#include "hello.h"

socket_controller::socket_controller(const std::string& ip, const std::string& port)
	: authorization_done(false)
	, connect_socket_(INVALID_SOCKET)
	, ip_(ip)
	, port_(port)
	, result_address_(nullptr)
	, receive_buffer_length_(default_buff_length)
{
	set_tcp_hints_values();
	initialize_winsock();
	resolve_ip_address_and_port();
	receive_buffer_ = new char[default_buff_length];
}


socket_controller::~socket_controller()
{
	freeaddrinfo(result_address_);
	delete receive_buffer_;
}



int socket_controller::initialize_protocol()
{

	// Attempt to connect to an address until one succeeds
	connect_to_server();
	check_connection();
	send_message(hello(std::string("kartonex12"), 12).to_string());

	try
	{
		while(!authorization_done)
		{
			// Receive until the peer closes the connection
			const auto result = recv(connect_socket_, receive_buffer_, receive_buffer_length_, 0);
			check_receive_result(result);
		}
	}
	catch(std::exception& e)
	{
		closesocket(connect_socket_);
		shut_down_socket(connect_socket_, SD_BOTH);
		return -1;
	}

	// shutdown the connection since no more data will be sent
	
	shut_down_socket(connect_socket_, SD_BOTH);

	return 0;
}

void socket_controller::check_receive_result(const int result) const
{
	if (result == 0 && !authorization_done || result < 0)
		throw std::exception(); //TODO zmieñ na custom exception

	// TODO zmien na wyjatek tutaj bardziej w sumie chociaz sam nie wiem
	// gowniane to sterowanie
}

void socket_controller::shut_down_socket(SOCKET socket, int mode)
{
	const auto result = shutdown(socket, mode);
	clean_up(socket);
	if(result == SOCKET_ERROR)
		throw std::runtime_error("Error on shutting down socket.");
}

void socket_controller::clean_up(SOCKET socket)
{
	closesocket(socket);
	WSACleanup();
}

void socket_controller::set_tcp_hints_values()
{
	// Fill memory with zeros, starts on hint's address and fills next siezof(hints_) with zeros
	ZeroMemory(&hints_, sizeof(hints_));
	hints_.ai_family = AF_UNSPEC;
	hints_.ai_socktype = SOCK_STREAM;
	hints_.ai_protocol = IPPROTO_TCP;
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
	const auto result = getaddrinfo(ip_.c_str(), port_.c_str(), &hints_, &result_address_);
	if (result != 0) {
		WSACleanup();
		throw std::runtime_error(std::string("getaddrinfo failed with error: ") + std::to_string(result));
	}
}

void socket_controller::check_connection() const
{
	if (connect_socket_ == INVALID_SOCKET) {
		WSACleanup();
		throw std::runtime_error(std::string("socket failed with error: ") + std::to_string(WSAGetLastError()));
	}
}

bool socket_controller::check_socket_error(const int status)
{
	if (status == SOCKET_ERROR)
	{
		closesocket(connect_socket_);
		connect_socket_ = INVALID_SOCKET;
		return true;
	}
	return false;
}

void socket_controller::connect_to_server()
{
	for (struct addrinfo *ptr = result_address_; ptr != nullptr; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		connect_socket_ = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		check_connection();

		// Connect to server.
		if (!check_socket_error(connect(connect_socket_, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen))))
		{
			break;
		}
	}
}

void socket_controller::send_message(std::string message) const
{
	// Send an initial buffer
	const auto result = send(connect_socket_, message.c_str(), message.size(), 0);
	if (result == SOCKET_ERROR) {
		clean_up(connect_socket_);
		throw std::runtime_error(std::string("send failed with error: ") + std::to_string(WSAGetLastError()));
	}
}
