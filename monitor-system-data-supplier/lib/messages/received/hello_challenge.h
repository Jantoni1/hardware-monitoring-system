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


	int id() const
	{
		return id_;
	}

	const std::string &challenge() const
	{
		return challenge_;
	}

	void consume(protocol_controller& controller) override;

private:
	int id_;
	const std::string challenge_;
	
};

