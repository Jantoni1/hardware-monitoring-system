#pragma once
#include <functional>
#include "received/message_received.h"
#include <unordered_map>
#include "received/access_denied.h"
#include "received/connected.h"
#include "received/hello_challenge.h"

enum class message_received_type {hello_challenge = 0, connected = 1, access_denied = 2};

class message_received_factory
{
public:
	message_received_factory() = default;
	~message_received_factory() = default;

	message_received* build_message(const char* buffer, const unsigned length) const;

private:

	access_denied* build_access_denied_message(const char* buffer, unsigned length) const;
	connected* build_connected_message(const char* buffer, unsigned length) const;
	hello_challenge* build_hello_challenge_message(const char* buffer, unsigned length) const;


	const std::unordered_map<message_received_type, std::function<message_received*(const char*, unsigned)>> factory_object_types_ =
	{ {message_received_type::hello_challenge, std::bind(&message_received_factory::build_hello_challenge_message, this
	                                                    , std::placeholders::_1, std::placeholders::_2)}
	, {message_received_type::connected, std::bind(&message_received_factory::build_connected_message, this
													    , std::placeholders::_1, std::placeholders::_2)}
	, {message_received_type::access_denied, std::bind(&message_received_factory::build_access_denied_message, this
														, std::placeholders::_1, std::placeholders::_2)} };
};

