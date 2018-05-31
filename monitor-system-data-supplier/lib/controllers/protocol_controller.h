#pragma once
#include "message_received_factory.h"
#include "tcp_socket_controller.h"
#include "udp_socket_controller.h"
#include "configuration.h"
#include "md5.h"

const static std::string standard_file_path = "config.cfg";

class protocol_controller
{
public:
	protocol_controller()
		: current_stage_(message_received_type::hello_challenge)
		, tcp_server_socket_controller_(nullptr)
		, tcp_daemon_socket_controller_(nullptr)
		, udp_server_socket_controller_(nullptr)
		, udp_thread_(nullptr)
	{}
	~protocol_controller();

	void consume(message_received *message_received_object);
	void consume(hello_challenge *hello_challenge_object);
	void consume(access_denied *access_denied_object);
	void consume(connected *connected_object);

	void establish_connection();

	void set_socket_controller(tcp_socket_controller *socket_controller_object)
	{
		tcp_server_socket_controller_ = socket_controller_object;
	}

	void set_configuration(const std::string &file_path = standard_file_path);

private:

	void run_udp_thread(); 
	void udp_send_loop() const;
	void open_connections();
	void shut_down_connections();
	void delete_sockets();
	void authorize();
	void wait_for_server_to_close_connection();
	void open_daemon_tcp_connection();
	void open_server_tcp_connection();

	std::string calculate_md5(const std::string& input);


	message_received_type current_stage_;
	message_received_factory message_received_factory_;

	tcp_socket_controller *tcp_server_socket_controller_;
	tcp_socket_controller *tcp_daemon_socket_controller_;
	udp_socket_controller *udp_server_socket_controller_;
	
	configuration configuration_;

	boost::thread *udp_thread_;

	MD5 md5_calculator;
};

