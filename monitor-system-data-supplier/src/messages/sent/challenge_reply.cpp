#include "stdafx.h"
#include <string>
#include "challenge_reply.h"
#include <Winsock2.h>


challenge_reply::challenge_reply(const int32_t id, std::string reply) 
	: id_(id)
	, reply_(std::move(reply))
{}

challenge_reply::~challenge_reply()
{
}

std::string challenge_reply::to_string() const
{
	char data[24];
	convert_message_type(data, message_sent_type::challenge_reply);
	convert_integer(data + 4, id_);
	copy_string(data + 8, reply_, 16);

	return  std::string(data);
}
