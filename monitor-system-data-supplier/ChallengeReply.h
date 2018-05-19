#pragma once
#include "SendMessage.h"
class ChallengeReply :
	public SendMessage
{
public:
	ChallengeReply();
	virtual ~ChallengeReply() override;

	virtual std::string toString() override;

private:
	int id_;
	std::string reply_;
};

