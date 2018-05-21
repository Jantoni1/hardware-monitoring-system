#pragma once
#include <Winsock2.h>
#include <string>

const int default_buff_length = 24;

class socket_controller
{
public:
	socket_controller(const std::string& ip, const std::string& port);
	~socket_controller();

	int initialize_protocol();

	bool authorization_done;

private:
	void check_receive_result(const int receive) const;
	static void shut_down_socket(SOCKET socket, int mode);
	static void clean_up(SOCKET socket);

	void set_tcp_hints_values();
	void initialize_winsock();
	void resolve_ip_address_and_port();
	void check_connection() const;
	bool check_socket_error(const int status);
	void connect_to_server();

	void send_message(std::string message) const;


	std::string ip_;
	std::string port_;
	WSADATA wsaData;
	SOCKET connect_socket_;
	struct addrinfo hints_;
	struct addrinfo *result_address_;
	char *receive_buffer_;
	int receive_buffer_length_;
};

