#pragma once
#include "message_sent.h"
class data :
	public message_sent
{
public:
	explicit data(const int32_t id)
		: id_(id)
		, temperature_(1)
		, timestamp_(1)
	{}
	~data() override;

	std::string to_string() const override;


	void set_temperature(const int temperature)
	{
		temperature_ = temperature;
		raw_data_.clear();
	}

	void set_timestamp(const uint64_t timestamp)
	{
		timestamp_ = timestamp;
		raw_data_.clear();
	}

	void set_raw_data(std::string&& raw_data)
	{
		raw_data_ = std::move(raw_data);
	}

	const std::string& raw_data() const
	{
		return raw_data_;
	}

	int id() const
	{
		return id_;
	}

	int temperature() const
	{
		return temperature_;
	}

	uint64_t timestamp() const
	{
		return timestamp_;
	}

private:
	int id_;
	int temperature_;
	uint64_t timestamp_;
	std::string raw_data_;
};

