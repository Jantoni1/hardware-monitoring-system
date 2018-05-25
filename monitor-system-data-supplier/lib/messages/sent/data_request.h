#pragma once
#include "C:\Users\Kuba\Documents\Visual Studio 2015\Projects\monitor-system-data-supplier\monitor-system-data-supplier\lib\messages\sent\message_sent.h"
class data_request :
	public message_sent
{
public:
	data_request();
	~data_request() override;

	std::string to_string() const override;
};

