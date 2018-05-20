#include "stdafx.h"
#include "data.h"

data::~data()
{
}

std::string data::to_string() const
{
	char data[20];

	convert_message_type(data, message_sent_type::data);
	convert_integer(data + 4, id_);
	convert_integer(data + 8, temperature_);
	convert_unsigned_long(data + 12, timestamp_);

	return  std::string(data);
}
