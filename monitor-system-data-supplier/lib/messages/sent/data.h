#pragma once
#include "message_sent.h"
class data :
	public message_sent
{
public:
	data(const int32_t id)
		: id_(id)
		, temperature_(1)
		, timestamp_(1)
	{}
	~data() override;

	std::string to_string() const override;


	void set_temperature(const int temperature)
	{
		temperature_ = temperature;
	}

	void set_timestamp(const uint64_t timestamp)
	{
		timestamp_ = timestamp;
	}

private:
	int id_;
	int temperature_;
	uint64_t timestamp_;
};

