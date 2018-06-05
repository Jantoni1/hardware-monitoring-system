#pragma once
#include "message_received.h"


class access_denied :
	public message_received
{
public:
	access_denied() = default;
	virtual ~access_denied() override = default;


	void consume(protocol_controller& controller) override;
};

