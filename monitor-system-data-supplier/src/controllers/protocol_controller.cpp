#include "stdafx.h"
#include "protocol_controller.h"
#include "challenge_reply.h"
#include "hello.h"
#include "data_request.h"

void wait(const int seconds)
{
	boost::this_thread::sleep_for(boost::chrono::seconds{ seconds });
}


protocol_controller::~protocol_controller()
{
	delete udp_thread_;
}

void protocol_controller::consume(message_received* message_received_object)
{
	throw std::runtime_error("Protocol error: received message of message_received");
}

void protocol_controller::consume(hello_challenge* hello_challenge_object)
{
	if(current_stage_ != message_received_type::hello_challenge)
	{
		throw std::runtime_error("Unexpectedly received hello_challenge message. Connection aborted.");
	}
	configuration_.set_id_from_server(hello_challenge_object->id());
	challenge_reply challenge_reply_object(configuration_.id_from_server(), calculate_md5(hello_challenge_object->challenge()));
	tcp_server_socket_controller_->send_message(challenge_reply_object.to_string());
	current_stage_ = message_received_type::connected;
}

void protocol_controller::consume(access_denied* access_denied_object)
{
	if (current_stage_ != message_received_type::connected)
	{
		throw std::runtime_error("Unexpectedly received access_denied message. Connection aborted.");
	}
	throw std::runtime_error("Access denied.");
}

void protocol_controller::consume(connected* connected_object)
{
	if (current_stage_ != message_received_type::connected)
	{
		throw std::runtime_error("Unexpectedly received connected message. Connection aborted.");
	}
	configuration_.set_authorization_passed(true);
}

void protocol_controller::establish_connection()
{
	try
	{
		open_connections();
		authorize();
		run_udp_thread();
		wait_for_server_to_close_connection();
	}
	catch(std::exception &e)
	{
		shut_down_connections();
		throw e;
	}
}

void protocol_controller::set_configuration(const std::string& file_path)
{
	configuration_.set_configuration_file_path(file_path);
	configuration_.parse_configuration();
	std::cout << " Configuration parsed from file: " << file_path << std::endl;
}

void protocol_controller::run_udp_thread()
{
	udp_server_socket_controller_ = new udp_socket_controller(configuration_.udp_server_ip_address(), configuration_.udp_server_port());
	udp_server_socket_controller_ -> initialize_udp_connection();
	std::cout << "Data channel opened at " << configuration_.udp_server_ip_address() << ":" << configuration_.udp_server_port() << std::endl;
	udp_thread_ = new boost::thread{ boost::bind(&protocol_controller::udp_send_loop, this) };
}

void protocol_controller::udp_send_loop() const
{
	try
	{
		data message(configuration_.id_from_server());
		while (true)
		{
			tcp_daemon_socket_controller_->send_message(data_request().to_string());
			message.set_raw_data(tcp_daemon_socket_controller_->receive());
			udp_server_socket_controller_->send(message);
			wait(1);
		}
	}
	catch(const boost::thread_interrupted &)
	{
	}

}

void protocol_controller::open_connections()
{
	open_daemon_tcp_connection();
	open_server_tcp_connection();
}

void protocol_controller::shut_down_connections()
{
	if (udp_thread_ != nullptr)
	{
		udp_thread_->interrupt();
		udp_thread_->join();
	}
	tcp_daemon_socket_controller_->shut_down_socket();
	tcp_server_socket_controller_->shut_down_socket();
	delete_sockets();
}

void protocol_controller::delete_sockets()
{
	delete udp_thread_;
	delete tcp_server_socket_controller_;
	delete tcp_daemon_socket_controller_;
	delete udp_server_socket_controller_;
	udp_thread_ = nullptr;
	tcp_server_socket_controller_ = nullptr;
	tcp_daemon_socket_controller_ = nullptr;
	udp_server_socket_controller_ = nullptr;
}

void protocol_controller::authorize()
{
	//send initial message
	tcp_server_socket_controller_->send_message(hello(configuration_.identification_name(), configuration_.id()).to_string());

	while (!configuration_.authorization_passed())
	{
		const auto datagram = std::move(tcp_server_socket_controller_->receive());
		consume(message_received_factory_.build_message(datagram.c_str(), datagram.size()));
	}
}

void protocol_controller::wait_for_server_to_close_connection()
{
	while (true)
	{
		tcp_server_socket_controller_->receive();
	}
}

void protocol_controller::open_daemon_tcp_connection()
{
	tcp_daemon_socket_controller_ = new tcp_socket_controller(
		configuration_.tcp_daemon_ip_address(), configuration_.tcp_daemon_port());

	std::cout << "Connecting to daemon..." << std::endl;
	tcp_daemon_socket_controller_->initialize_protocol();
	std::cout << "Connection with daemon opened at "
		<< configuration_.tcp_daemon_ip_address() << ":" << configuration_.tcp_daemon_port() << std::endl;
}

void protocol_controller::open_server_tcp_connection()
{
	tcp_server_socket_controller_ = new tcp_socket_controller(
		configuration_.tcp_server_ip_address(), configuration_.tcp_server_port());

	std::cout << "Connecting to server..." << std::endl;
	tcp_server_socket_controller_->initialize_protocol();
	std::cout << "Connection with server opened at "
		<< configuration_.tcp_server_ip_address() << ":" << configuration_.tcp_server_port() << std::endl;
}

std::string protocol_controller::calculate_md5(const std::string& input)
{
	return input; //TODO implement
}

