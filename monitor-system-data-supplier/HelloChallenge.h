#pragma once
#include "ReceivedMessage.h"
#include <string>

class HelloChallenge :
	public ReceivedMessage
{
public:
	HelloChallenge(const int id,const std::string& challenge) 
		: id_(id)
		, challenge_(challenge)
	{}
	virtual ~HelloChallenge() override = default;

private:
	int id_;
	const std::string challenge_;
	
};

