#pragma once
#include <string>



/**
* @brief abstract base class for messages sent by this app
*  */
class SendMessage
{
public:
	SendMessage() = default;
	virtual ~SendMessage() = 0;

	virtual std::string toString() = 0;
};

