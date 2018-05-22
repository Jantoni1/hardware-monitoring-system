#include "stdafx.h"
#include "message_received_factory.h"
#include "empty_data_buffer_exception.h"
#include "message_received.h"
#include <Winsock2.h>
#include "corrupted_message_exception.h"

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

int32_t convert_to_int(char* integer)
{
	int32_t_converter converter;
	memcpy(converter.char_array, integer, 4);
	return ntohl(converter.int_value);
}


message_received* message_received_factory::build_message(char* buffer, const unsigned length) const
{
	if(length == 0)
	{
		throw empty_data_buffer_exception();
	}

	return factory_object_types_.at(static_cast<message_received_type>(buffer[1]))(buffer, length);
}

access_denied* message_received_factory::build_access_denied_message(char* buffer, unsigned length) const
{
	return new access_denied();
}

connected* message_received_factory::build_connected_message(char* buffer, unsigned length) const
{
	return new connected();
}

hello_challenge* message_received_factory::build_hello_challenge_message(char* buffer, const unsigned length) const
{
	if(length < 20)
	{
		throw corrupted_message_exception();
	}
	return new hello_challenge(convert_to_int(buffer), std::string(buffer + 4, length - 4));
}

