#pragma once
#include "message_sent.h"
class data :
	public message_sent
{
public:
	data(const int32_t id, const int32_t temperature, const uint64_t timestamp)
		: id_(id)
		, temperature_(temperature)
		, timestamp_(timestamp)
	{}
	~data() override;

	std::string to_string() const override;

private:
	int id_;
	int temperature_;
	uint64_t timestamp_;
};

