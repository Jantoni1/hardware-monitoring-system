#pragma once
#include <Winsock2.h>
#include <string>

class socket_controller
{
public:
	socket_controller(const std::string& ip, const std::string& port);
	~socket_controller();

	int connect1();

	bool authorization_done;

private:
	void check_receive_result(int receive) const;
	void shut_down_socket(SOCKET socket, int mode);
	bool check_socket_operation_error(SOCKET socket, int result);
	void clean_up(SOCKET socket);

	void set_tcp_hints_values();
	void initialize_winsock();
	void resolve_ip_address_and_port();

	std::string ip_;
	std::string port_;
	WSADATA wsaData;
	SOCKET ConnectSocket;
	struct addrinfo hints;
	struct addrinfo *result_address_;
};

