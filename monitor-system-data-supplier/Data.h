#pragma once
#include "SendMessage.h"
class Data :
	public SendMessage
{
public:
	Data();
	virtual ~Data();

	virtual std::string toString() override;

private:
	int id_;
	int temperature_;
	uint64_t timestamp_;
};

