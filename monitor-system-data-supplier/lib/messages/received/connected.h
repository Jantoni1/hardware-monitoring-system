#pragma once
#include "message_received.h"
class connected :
	public message_received
{
public:
	connected() = default;
	virtual ~connected() override = default;
};
