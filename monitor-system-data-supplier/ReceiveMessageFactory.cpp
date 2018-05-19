#include "stdafx.h"
#include "ReceiveMessageFactory.h"
#include "EmptyDataBufferException.h"

ReceivedMessage* ReceiveMessageFactory::buildMessage(char* buffer, unsigned length) const
{
	if(length == 0)
	{
		throw EmptyDataBufferException();
	}

	ReceivedMessage *message = (factory_object_types_[static_cast<ReceivedMessageType>(buffer[1])])(buffer, length);
}
