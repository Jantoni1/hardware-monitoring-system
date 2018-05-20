#include "stdafx.h"
#include "message_sent.h"
#include <Winsock2.h>

void message_sent::convert_integer(char *dest, const uint32_t integer_variable)
{
	memcpy(dest, reinterpret_cast<char*>(htonl(integer_variable)), 4);
}

void message_sent::convert_unsigned_long(char* dest, const uint64_t long_variable)
{
	memcpy(dest, reinterpret_cast<char*>(htonll(long_variable)), 8);
}

void message_sent::convert_message_type(char* dest, const message_sent_type type)
{
	memcpy(dest, reinterpret_cast<char*>(htonl(static_cast<int32_t>(type))), 4);
}

void message_sent::copy_string(char* dest, const std::string src, const int32_t length)
{
	memcpy(dest, src.c_str(), length);
}

message_sent::~message_sent ()
{
	
}
