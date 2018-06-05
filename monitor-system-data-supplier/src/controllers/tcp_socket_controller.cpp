#include "stdafx.h"
#include "controllers/tcp_socket_controller.h"

//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include <iostream>


tcp_socket_controller::tcp_socket_controller(const std::string& ip, const std::string& port)
	: authorization_done_(false)
	, tcp_socket_(INVALID_SOCKET)
	, tcp_ip_(ip)
	, tcp_port_(port)
	, result_address_(nullptr)
	, receive_buffer_length_(default_buff_length)
	, udp_thread_(nullptr)
{
	set_tcp_hints_values();
	initialize_winsock();
	resolve_ip_address_and_port();
	receive_buffer_ = new char[default_buff_length];
}


tcp_socket_controller::~tcp_socket_controller()
{
	freeaddrinfo(result_address_);
	delete receive_buffer_;
	delete udp_thread_;
}



void tcp_socket_controller::initialize_protocol()
{
	// Attempt to connect to an address until one succeeds
	connect_to_server();
	check_connection();
}

std::pair<int, std::string> tcp_socket_controller::receive() const
{
	int res = check_receive_result(recv(tcp_socket_, receive_buffer_, receive_buffer_length_, 0));
	return{ res, std::string(receive_buffer_, receive_buffer_length_) };
}

int tcp_socket_controller::check_receive_result(const int result) const
{
	if(result == 0 && authorization_done_)
	{
		throw std::range_error("End of transmission");
	}
	if (result == 0 && !authorization_done_ || result < 0)
	{
		if (WSAGetLastError() != WSAETIMEDOUT)
		{
			throw std::runtime_error("Protocol error, socket changed or client failed to authorize.");
		}
		return -1;
	}
	return 0;
}

void tcp_socket_controller::shut_down_socket() const
{
	const auto result = shutdown(tcp_socket_, SD_BOTH);
	clean_up();
}

void tcp_socket_controller::clean_up() const
{
	closesocket(tcp_socket_);
	WSACleanup();
}

void tcp_socket_controller::set_tcp_hints_values()
{
	// Fill memory with zeros, starts on hint's address and fills next siezof(hints_) with zeros
	ZeroMemory(&hints_, sizeof(hints_));
	hints_.ai_family = AF_UNSPEC;
	hints_.ai_socktype = SOCK_STREAM;
	hints_.ai_protocol = IPPROTO_TCP;
}

void tcp_socket_controller::initialize_winsock()
{
	// Initialize Winsock
	// Retreieves implementation data from underlying windows OS
	const auto result = WSAStartup(MAKEWORD(2, 2), &tcp_wsa_data_);
	if (result != 0) {
		throw std::runtime_error(std::string("WSAStartup failed with error: ") + std::to_string(result));
	}
}

void tcp_socket_controller::resolve_ip_address_and_port()
{
	// Resolve the server address and port
	const auto result = getaddrinfo(tcp_ip_.c_str(), tcp_port_.c_str(), &hints_, &result_address_);
	if (result != 0) {
		WSACleanup();
		throw std::runtime_error(std::string("getaddrinfo failed with error: ") + std::to_string(result));
	}
}

void tcp_socket_controller::check_connection() const
{
	if (tcp_socket_ == INVALID_SOCKET) {
		WSACleanup();
		throw std::runtime_error(std::string("socket failed with error: ") + std::to_string(WSAGetLastError()));
	}
	DWORD timeout = 1000;
	setsockopt(tcp_socket_, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
}

bool tcp_socket_controller::check_socket_error(const int status)
{
	if (status == SOCKET_ERROR)
	{
		closesocket(tcp_socket_);
		tcp_socket_ = INVALID_SOCKET;
		return true;
	}
	return false;
}

void tcp_socket_controller::connect_to_server()
{
	for (struct addrinfo *ptr = result_address_; ptr != nullptr; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		tcp_socket_ = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		check_connection();

		// Connect to server.
		if (!check_socket_error(connect(tcp_socket_, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen))))
		{
			break;
		}
	}
}

void tcp_socket_controller::send_message(std::string message) const
{
	const auto result = send(tcp_socket_, message.c_str(), message.size(), 0);
	if (result == SOCKET_ERROR) {
		clean_up();
		throw std::runtime_error(std::string("send failed with error: ") + std::to_string(WSAGetLastError()));
	}
}

