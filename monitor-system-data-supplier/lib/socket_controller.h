#pragma once
#include <Winsock2.h>
#include <string>
#include "message_received_factory.h"
#include "protocol_controller.h"
#include <boost/thread.hpp>

class data;
const int default_buff_length = 24;

class socket_controller
{
public:
	socket_controller(const std::string& tcp_ip, const std::string& tcp_port, const std::string& udp_ip, const std::string& udp_port);
	~socket_controller();

	void initialize_protocol();
	void send_message(std::string message) const;
	void authorization_done();
	void run_udp_thread();

private:
	void check_receive_result(const int receive);
	static void shut_down_socket(SOCKET socket, int mode);
	static void clean_up(SOCKET socket);

	void set_tcp_hints_values();
	void initialize_winsock(WORD word, WSADATA *wsa_data) const;
	void resolve_ip_address_and_port();
	void check_connection() const;
	bool check_socket_error(const int status);
	void connect_to_server();

	void receive_loop();
	void send_answer();

	void udp_send_loop();
	void set_udp_server_address();
	void set_udp_server_port();
	void fill_udp_server_address_structure();
	void create_udp_socket();
	void bind_client_address_and_port_to_udp_socket();

	std::string get_new_data(data &data_object) const;
	void initialize_udp_connection();

	message_received_factory message_received_factory_;
	protocol_controller protocol_controller_;

	std::string tcp_ip_;
	std::string tcp_port_;

	WSADATA tcp_wsa_data_;
	WSADATA udp_wsa_data_;
	SOCKET tcp_socket_;
	int udp_socket_;
	struct addrinfo hints_;
	struct addrinfo *result_address_;
	char *receive_buffer_;
	int receive_buffer_length_;
	bool authorization_done_;

	std::string udp_ip_;
	std::string udp_port_;
	struct sockaddr_in udp_server_address_; /*server address*/
	struct sockaddr_in udp_client_addrress_; /*client address*/

	boost::thread* udp_thread_;

	const int client_port_ = 2137;
};

