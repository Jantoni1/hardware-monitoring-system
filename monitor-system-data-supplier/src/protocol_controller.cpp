#include "stdafx.h"
#include "protocol_controller.h"
#include "challenge_reply.h"


protocol_controller::~protocol_controller()
{
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
	id_ = hello_challenge_object->id();
	challenge_reply challenge_reply_object(hello_challenge_object->id(), calculate_md5(hello_challenge_object->challenge()));
	socket_controller_->send_message(challenge_reply_object.to_string());
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
	socket_controller_->authorization_done();
	socket_controller_->run_udp_thread();
}

std::string protocol_controller::calculate_md5(const std::string& input)
{
	return input; //TODO implement
}
