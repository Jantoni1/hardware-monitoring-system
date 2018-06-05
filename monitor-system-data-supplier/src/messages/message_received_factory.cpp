
#include "message_received_factory.h"
#include "message_received.h"
#include <Winsock2.h>
#include <iostream>

union int32_t_converter
{
	char char_array[4];
	int32_t int_value;
};

union uint64_t_converter
{
	char char_array[4];
	uint64_t long_value;
};

int32_t convert_to_int(const char* integer)
{
	int32_t_converter converter;
	memcpy(converter.char_array, integer, 4);
	return ntohl(converter.int_value);
}


message_received* message_received_factory::build_message(const char* buffer, const unsigned length) const
{
	if(length < 4)
	{
		throw std::runtime_error("Unexpectedly received empty message.");
	}
	const auto descriptor = convert_to_int(buffer);
	return factory_object_types_.at(static_cast<message_received_type>(descriptor))(buffer, length);
}

access_denied* message_received_factory::build_access_denied_message(const char* buffer, unsigned length) const
{
	return new access_denied();
}

connected* message_received_factory::build_connected_message(const char* buffer, unsigned length) const
{
	return new connected();
}

hello_challenge* message_received_factory::build_hello_challenge_message(const char* buffer, const unsigned length) const
{
	if(length < 24)
	{
		throw std::runtime_error("Received corrupted message during authentication process.");
	}
	return new hello_challenge(convert_to_int(buffer + 4), std::string(buffer + 8, length - 8));
}

