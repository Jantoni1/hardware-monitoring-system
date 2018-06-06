#pragma once
#include "message_sent.h"
class challenge_reply :
	public message_sent
{
public:
	challenge_reply( std::string challenge);
	~challenge_reply() override;

	std::string to_string() const override;

private:
	std::string reply_;
};

