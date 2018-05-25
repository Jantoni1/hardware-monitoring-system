#include "stdafx.h"
#include "controllers/udp_socket_controller.h"
#include "data.h"
#include <boost/thread.hpp>
#include <ws2tcpip.h>


udp_socket_controller::~udp_socket_controller()
{

}

void udp_socket_controller::initialize_winsock()
{
	// Initialize Winsock
	// Retreieves implementation data from underlying windows OS
	const auto result = WSAStartup(MAKEWORD(2, 2), &udp_wsa_data_);
	if (result != 0) {
		throw std::runtime_error(std::string("WSAStartup failed with error: ") + std::to_string(result));
	}
}

void udp_socket_controller::send(const data &data_object)
{
	const auto message = std::move(data_object.to_string());
	sendto(udp_socket_, message.c_str(), message.size(), 0, reinterpret_cast<LPSOCKADDR>(&udp_server_address_), sizeof(udp_server_address_));
}

void udp_socket_controller::set_udp_server_address()
{
	const auto check =  inet_pton(AF_INET, udp_ip_.c_str(), &udp_server_address_.sin_addr.s_addr);/*get the ip address*/
	if (check != 1) {
		throw std::runtime_error("Cannot resolve udp server address.");
	}
}

void udp_socket_controller::set_udp_server_port()
{
	const auto server_port = atoi(udp_port_.c_str());
	if (server_port>0)
		udp_server_address_.sin_port = htons(static_cast<u_short>(server_port));/*get the port*/
	else {
		throw std::runtime_error("Inaccessible port or incorrect port input format (number required).");
	}
}

void udp_socket_controller::fill_udp_server_address_structure()
{
	memset(reinterpret_cast<char*>(&udp_client_addrress_), 0, sizeof(udp_client_addrress_));
	udp_client_addrress_.sin_family = AF_INET; /*set client address protocol family*/
	udp_client_addrress_.sin_addr.s_addr = INADDR_ANY;
	udp_client_addrress_.sin_port = htons(static_cast<u_short>(client_port_)); /*set client port*/
	udp_server_address_.sin_family = AF_INET;
}

void udp_socket_controller::create_udp_socket()
{
	udp_socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);/*create a socket*/
	if (udp_socket_ < 0) {
		throw std::runtime_error("Socket create attempt has failed.");
	}
}

void udp_socket_controller::bind_client_address_and_port_to_udp_socket()
{
	if (bind(udp_socket_, reinterpret_cast<LPSOCKADDR>(&udp_client_addrress_), sizeof(udp_client_addrress_)) < 0) {/*bind a client address and port*/
		throw std::runtime_error("UDP socket bind failed.");
	}
}

void udp_socket_controller::initialize_udp_connection()
{
	initialize_winsock();
	fill_udp_server_address_structure();
	set_udp_server_address();
	set_udp_server_port();
	create_udp_socket();
	bind_client_address_and_port_to_udp_socket();
}

void udp_socket_controller::clean_up(SOCKET socket)
{
	closesocket(socket);
	WSACleanup();
}