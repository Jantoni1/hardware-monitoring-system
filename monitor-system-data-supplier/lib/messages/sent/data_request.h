#pragma once
#include "message_sent.h"
class data_request :
	public message_sent
{
public:
	data_request();
	~data_request() override;

	std::string to_string() const override;
};

