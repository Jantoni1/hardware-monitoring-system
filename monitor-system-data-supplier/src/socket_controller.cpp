#include "stdafx.h"
#include "socket_controller.h"
#pragma warning(disable:4996) //for using inet_addr()

//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <boost/algorithm/string.hpp>
#include "hello.h"
#include "data.h"

void wait(const int seconds)
{
	boost::this_thread::sleep_for(boost::chrono::seconds{ seconds });
}

socket_controller::socket_controller(const std::string& ip, const std::string& port, const std::string& udp_ip, const std::string& udp_port)
	: authorization_done_(false)
	, tcp_socket_(INVALID_SOCKET)
	, tcp_ip_(ip)
	, tcp_port_(port)
	, udp_ip_(udp_ip)
	, udp_port_(udp_port)
	, result_address_(nullptr)
	, receive_buffer_length_(default_buff_length)
	, udp_thread_(nullptr)
{
	protocol_controller_.set_socket_controller(this);
	set_tcp_hints_values();
	initialize_winsock(MAKEWORD(2, 2), &tcp_wsa_data_);
	resolve_ip_address_and_port();
	receive_buffer_ = new char[default_buff_length];
}


socket_controller::~socket_controller()
{
	freeaddrinfo(result_address_);
	delete receive_buffer_;
	delete udp_thread_;
}



void socket_controller::initialize_protocol()
{
	// Attempt to connect to an address until one succeeds
	connect_to_server();
	check_connection();
	//send initial message
	send_message(hello(std::string("kartonex12"), 12).to_string());

	//receive and send messages via this function
	receive_loop();
}

void socket_controller::receive_loop() 
{
	try
	{
		auto result = -1;
		while (result != 0)
		{
			// Receive until the peer closes the connection
			result = recv(tcp_socket_, receive_buffer_, receive_buffer_length_, 0);
			check_receive_result(result);
		}
	}
	catch (std::exception& e)
	{
		closesocket(tcp_socket_);
	}
	// shutdown the connection since no more data will be sent
	shut_down_socket(tcp_socket_, SD_BOTH);
}

void socket_controller::send_answer()
{
	auto* message_received_object = message_received_factory_.build_message(receive_buffer_, receive_buffer_length_);
	try
	{
		protocol_controller_.consume(message_received_object);
		delete message_received_object;
	}
	catch (std::exception &e)
	{
		delete message_received_object;
		throw e;
	}
}

void socket_controller::udp_send_loop()
{
	try
	{
		//initialize socket
		initialize_udp_connection();
		data data_object(protocol_controller_.id());
		while (true)
		{
			const auto message = std::move(get_new_data(data_object));
			sendto(udp_socket_, message.c_str() , message.size(), 0, (LPSOCKADDR)&udp_server_address_, sizeof(struct sockaddr));
			wait(1); //TODO implement UDP communication and retreiving hardware temperature data
		}

	}
	catch (boost::thread_interrupted&)
	{}
	catch (std::runtime_error& e)
	{}
	clean_up(udp_socket_);

}

void socket_controller::set_udp_server_address()
{
	udp_server_address_.sin_addr.s_addr = inet_pton(AF_INET, udp_ip_.c_str(), &udp_server_address_.sin_addr);/*get the ip address*/
	if (udp_server_address_.sin_addr.s_addr != 1) {
		throw std::runtime_error("Cannot resolve udp server address.");
	}
}

void socket_controller::set_udp_server_port()
{
	const auto server_port = atoi(udp_port_.c_str());
	if (server_port>0)
		udp_server_address_.sin_port = htons(static_cast<u_short>(server_port));/*get the port*/
	else {
		throw std::runtime_error("Inaccessible port or incorrect port input format (number required).");
	}
}

void socket_controller::fill_udp_server_address_structure()
{
	memset((char*)&udp_client_addrress_, 0, sizeof(udp_client_addrress_));
	udp_client_addrress_.sin_family = AF_INET; /*set client address protocol family*/
	udp_client_addrress_.sin_addr.s_addr = INADDR_ANY;
	udp_client_addrress_.sin_port = htons(static_cast<u_short>(client_port_)); /*set client port*/
	udp_server_address_.sin_family = AF_INET;
}

void socket_controller::create_udp_socket()
{
	udp_socket_ = socket(PF_INET, SOCK_DGRAM, 0);/*create a socket*/
	if (udp_socket_ < 0) {
		throw std::runtime_error("Socket create attempt has failed.");
	}
}

void socket_controller::bind_client_address_and_port_to_udp_socket()
{
	if (bind(udp_socket_, (LPSOCKADDR)&udp_client_addrress_, sizeof(struct sockaddr)) < 0) {/*bind a client address and port*/
		throw std::runtime_error("UDP socket bind failed.");
	}
}

std::string socket_controller::get_new_data(data& data_object) const
{
	data_object.set_temperature(protocol_controller_.get_temperature());
	data_object.set_timestamp(protocol_controller_.get_timestamp());
	return data_object.to_string();
}

void socket_controller::initialize_udp_connection()
{
	initialize_winsock(0x0202, &udp_wsa_data_);
	fill_udp_server_address_structure();
	set_udp_server_address();
	set_udp_server_port();
	create_udp_socket();
	bind_client_address_and_port_to_udp_socket();
}

void socket_controller::check_receive_result(const int result)
{
	if(result == 0 && authorization_done_)
	{
		udp_thread_->interrupt();
		udp_thread_->join();
	}
	if (result == 0 && !authorization_done_ || result < 0)
		throw std::runtime_error("Protocol error, socket changed or client failed to authorize.");
	send_answer();
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

void socket_controller::initialize_winsock(WORD word, WSADATA *wsa_data) const
{
	// Initialize Winsock
	// Retreieves implementation data from underlying windows OS
	const auto result = WSAStartup(word, wsa_data);
	if (result != 0) {
		throw std::runtime_error(std::string("WSAStartup failed with error: ") + std::to_string(result));
	}
}

void socket_controller::resolve_ip_address_and_port()
{
	// Resolve the server address and port
	const auto result = getaddrinfo(tcp_ip_.c_str(), tcp_port_.c_str(), &hints_, &result_address_);
	if (result != 0) {
		WSACleanup();
		throw std::runtime_error(std::string("getaddrinfo failed with error: ") + std::to_string(result));
	}
}

void socket_controller::check_connection() const
{
	if (tcp_socket_ == INVALID_SOCKET) {
		WSACleanup();
		throw std::runtime_error(std::string("socket failed with error: ") + std::to_string(WSAGetLastError()));
	}
}

bool socket_controller::check_socket_error(const int status)
{
	if (status == SOCKET_ERROR)
	{
		closesocket(tcp_socket_);
		tcp_socket_ = INVALID_SOCKET;
		return true;
	}
	return false;
}

void socket_controller::connect_to_server()
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

void socket_controller::send_message(std::string message) const
{
	// Send an initial buffer
	const auto result = send(tcp_socket_, message.c_str(), message.size(), 0);
	if (result == SOCKET_ERROR) {
		clean_up(tcp_socket_);
		throw std::runtime_error(std::string("send failed with error: ") + std::to_string(WSAGetLastError()));
	}
}

void socket_controller::authorization_done()
{
	authorization_done_ = true;
}

void socket_controller::run_udp_thread()
{
	udp_thread_ = new boost::thread{ boost::bind(&socket_controller::udp_send_loop, this) };
}
