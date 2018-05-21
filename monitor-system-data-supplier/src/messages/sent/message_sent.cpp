#include "stdafx.h"
#include "message_sent.h"
#include <Winsock2.h>
#include <iostream>

//template<typename T>
//void convert_to_char(char* dest, const T t, const std::size_t size)
//{
//	for (int i = 0; i < size; ++i)
//	{
//		dest[i] = t >> i * 8;
//		std::cout << (int)dest[i] << std::endl;
//	}
//}

union int32_t_converter
{
	char char_array[4];
	int32_t int_value;
};

union int64_t_converter
{
	char char_array[4];
	uint64_t long_value;
};

void message_sent::convert_integer(char* dest, const int32_t integer_variable)
{
	int32_t_converter converter;
	converter.int_value = htonl(integer_variable);
	memcpy(dest, converter.char_array, 4);
//	convert_to_char<int32_t>(dest, integer_variable, 4);
}

void message_sent::convert_unsigned_long(char* dest, const uint64_t long_variable)
{
	int64_t_converter converter;
	converter.long_value = htonl(long_variable);
	memcpy(dest, converter.char_array, 8);
//	convert_to_char<uint64_t>(dest, long_variable, 8);
}

void message_sent::convert_message_type(char* dest, const message_sent_type type)
{
	//explicitly convert to int and change byte order with host-to-network-long-conversion function
	int32_t converted_type = htonl(static_cast<int32_t>(type));
	char* type_array = reinterpret_cast<char*>(&converted_type);
	memcpy(dest, type_array, 4);
}

void message_sent::copy_string(char* dest, const std::string src, const int32_t length)
{
	memcpy(dest, src.c_str(), length);
}

message_sent::~message_sent ()
{
	
}
