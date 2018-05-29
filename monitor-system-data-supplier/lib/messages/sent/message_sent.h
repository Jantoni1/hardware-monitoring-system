#pragma once
#include <string>

enum class message_sent_type {hello = 1, challenge_reply = 3, data  = 6};

/**
* @brief abstract base class for messages sent by this app
*  */
class message_sent
{
public:
	virtual ~message_sent() = 0;

	virtual std::string to_string() const = 0;

protected:
	static void convert_integer(char*, const int32_t integer_variable);
	static void convert_unsigned_long(char* dest, const uint64_t long_variable);
	static void convert_message_type(char* dest, const message_sent_type type);
	static void copy_string(char* dest, const std::string src, const int32_t length);
};

