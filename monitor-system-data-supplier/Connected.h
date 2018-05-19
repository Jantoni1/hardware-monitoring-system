#pragma once
#include "ReceivedMessage.h"
class Connected :
	public ReceivedMessage
{
public:
	Connected() = default;
	virtual ~Connected() override = default;
};

