#pragma once

/**
 * @brief abstract base class for received messages
 *  */

class protocol_controller;

class message_received
{
public:
	message_received() = default;
	virtual ~message_received() = 0;
	
	virtual void consume(protocol_controller& controller) = 0;
};
