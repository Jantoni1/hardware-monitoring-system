#pragma once
#include <Winsock2.h>
#include <string>
#include "data.h"

class udp_socket_controller
{
public:
	udp_socket_controller(const std::string& udp_ip, const std::string &udp_port) 
		: udp_ip_(udp_ip)
		, udp_port_(udp_port)
		, udp_socket_(-1)
	{}
	udp_socket_controller();
	~udp_socket_controller();

	void initialize_udp_connection();
	void send(const data &data_object);


private:
	WSADATA udp_wsa_data_;


	std::string udp_ip_;
	std::string udp_port_;
	struct sockaddr_in udp_server_address_; /*server address*/
	struct sockaddr_in udp_client_addrress_; /*client address*/

	const int client_port_ = 13005;
	
	int udp_socket_;

	void initialize_winsock();
	void set_udp_server_address();
	void set_udp_server_port();
	void fill_udp_server_address_structure();
	void create_udp_socket();
	void bind_client_address_and_port_to_udp_socket();
	static void clean_up(SOCKET socket);

};

