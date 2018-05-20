#pragma once
#include "message_sent.h"
class challenge_reply :
	public message_sent
{
public:
	challenge_reply(const int32_t id, std::string& reply);
	~challenge_reply() override;

	std::string to_string() const override;

private:
	int32_t id_;
	std::string reply_;
};

