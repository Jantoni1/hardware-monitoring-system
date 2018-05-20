#pragma once
#include "message_sent.h"
class hello :
	public message_sent
{
public:
	hello(const std::string& name, const int32_t idnr)
		: name_(name)
		, idnr_(idnr)
	{}
	~hello() override;

	std::string to_string() const override;

private:
	std::string name_; // char[10] length
	int32_t idnr_;
};

