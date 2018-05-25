#pragma once
#include <Winsock2.h>
#include <string>
#include <boost/thread.hpp>


class tcp_socket_controller
{
public:
	tcp_socket_controller(const std::string& tcp_ip, const std::string& tcp_port);
	~tcp_socket_controller();

	void initialize_protocol();
	void send_message(std::string message) const;
	std::string receive() const;
	void shut_down_socket() const;


private:
	void check_receive_result(const int receive) const;
	void clean_up() const;


	void set_tcp_hints_values();
	void initialize_winsock();
	void resolve_ip_address_and_port();
	void check_connection() const;
	bool check_socket_error(const int status);
	void connect_to_server();

	std::string tcp_ip_;
	std::string tcp_port_;

	WSADATA tcp_wsa_data_;
	SOCKET tcp_socket_;

	struct addrinfo hints_;
	struct addrinfo *result_address_;
	char *receive_buffer_;
	int receive_buffer_length_;
	bool authorization_done_;

	boost::thread* udp_thread_;


};

const int default_buff_length = 24;


