#pragma once
#include "vcruntime_exception.h"
class corrupted_message_exception :
	public std::exception
{
public:
	corrupted_message_exception();
	~corrupted_message_exception();
};

