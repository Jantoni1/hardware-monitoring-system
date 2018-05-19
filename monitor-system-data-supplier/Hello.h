#pragma once
#include "SendMessage.h"
class Hello :
	public SendMessage
{
public:
	Hello();
	virtual ~Hello();

	virtual std::string toString() override;

private:
	std::string name_;
	int idnr_;
};

