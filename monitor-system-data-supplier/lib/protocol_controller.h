#pragma once
#include "message_received_factory.h"
#include "socket_controller.h"

class protocol_controller
{
public:
	protocol_controller()
		: current_stage_(message_received_type::hello_challenge)
		, socket_controller_(nullptr)
		, id_(0)
	{}
	~protocol_controller();

	void consume(message_received *message_received_object);
	void consume(hello_challenge *hello_challenge_object);
	void consume(access_denied *access_denied_object);
	void consume(connected *connected_object);

	void set_socket_controller(socket_controller *socket_controller_object)
	{
		socket_controller_ = socket_controller_object;
	}

	int32_t get_temperature() const
	{
		return 0; //TODO implement
	}

	uint64_t get_timestamp() const
	{
		return 0; //TODO implement
	}

	int32_t id() const
	{
		return id_;
	}

private:
	static std::string calculate_md5(const std::string& input);


	message_received_type current_stage_;
	socket_controller *socket_controller_;

	int32_t id_;

};

