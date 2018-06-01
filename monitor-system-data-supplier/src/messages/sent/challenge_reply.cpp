#include "stdafx.h"
#include <string>
#include "challenge_reply.h"
#include <Winsock2.h>


challenge_reply::challenge_reply(std::string challenge) 
	: reply_(std::move(challenge))
{}

challenge_reply::~challenge_reply()
{
}

std::string challenge_reply::to_string() const
{
	char data[36];
	convert_message_type(data, message_sent_type::challenge_reply);
	copy_string(data + 4, reply_, 32);
	return  std::string(data, 36);
}
