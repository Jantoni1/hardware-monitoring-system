#pragma once
#include "message_received.h"
#include <string>

class hello_challenge :
	public message_received
{
public:
	hello_challenge(const int id,const std::string& challenge) 
		: id_(id)
		, challenge_(challenge)
	{}
	virtual ~hello_challenge() override = default;

private:
	int id_;
	const std::string challenge_;
	
};

