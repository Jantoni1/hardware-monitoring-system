#include "stdafx.h"
#include "hello.h"
#include <iostream>


hello::~hello()
{
}

std::string hello::to_string() const
{
	char data[18];

	convert_message_type(data, message_sent_type::hello);
	copy_string(data + 4, name_, 10);
	convert_integer(data + 14, idnr_);

	return std::string(data, 18);
}
