#pragma once
#include "ReceivedMessage.h"
class AccessDenied :
	public ReceivedMessage
{
public:
	AccessDenied() = default;
	virtual ~AccessDenied() override = default;
};

