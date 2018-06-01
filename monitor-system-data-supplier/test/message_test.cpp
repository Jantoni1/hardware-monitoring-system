#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "hello.h"
#include "data_request.h"
#include "data.h"
#include <iostream>
#include <Winsock2.h>
#include "challenge_reply.h"

#define BOOST_TEST_DYN_LINK

union int32_t_converter
{
	char char_array[4];
	int32_t int_value;
};

union uint64_t_converter
{
	char char_array[4];
	uint64_t long_value;
};

std::string create_hello_message(const std::string input, const int input_value)
{
	const auto input_string(input);
	hello hello_object(input_string, input_value);
	return hello_object.to_string();
}

int32_t copy_int_from_buffer(const std::string& str, const int position)
{
	int32_t_converter converter;
	memcpy(converter.char_array, str.c_str() + position, 4);
	return ntohl(converter.int_value);
}

std::string copy_string_from_buffer(const std::string& buffer, int position, const int bytes)
{
	char *tab = new char[bytes];
	memcpy(tab, buffer.c_str() + position, bytes);
	auto result = std::string(tab, bytes);
	delete[] tab;
	return result;
}

uint64_t copy_unsigned_long_from_buffer(const std::string& str, const int position)
{
	uint64_t_converter converter;
	memcpy(converter.char_array, str.c_str() + position, 8);
	return ntohll(converter.long_value);
}

std::string calculate_md5(const std::string& challenge)
{
	return challenge; //TODO
}

BOOST_AUTO_TEST_SUITE(building_hello_message)

BOOST_AUTO_TEST_CASE(build_sample_hello_message_retreived_message_type_equals_hello)
{
	message_sent_type type = message_sent_type::hello;
	const auto hello_output = create_hello_message("__input__.", 12);
	const auto retreived_message_sent_type = copy_int_from_buffer(hello_output, 0);
	BOOST_CHECK_EQUAL(retreived_message_sent_type, static_cast<int>(type));
}


BOOST_AUTO_TEST_CASE(build_sample_hello_message_check_if_output_is_24_bytes_long)
{
	const auto hello_output = create_hello_message("__input__.", 65534);
	BOOST_CHECK_EQUAL(hello_output.size(), 18);
}


BOOST_AUTO_TEST_CASE(build_sample_hello_message)
{
	const auto input_string("__input__.");
	const auto hello_output = create_hello_message(input_string, 2147483647);
	BOOST_CHECK_EQUAL(hello_output.compare(4, 10, input_string), 0);
}


BOOST_AUTO_TEST_CASE(build_sample_hello_message_retreived_id_value_equals_12) 
{
	const auto id = 12;
	const auto hello_output = create_hello_message("__input__.", id);
	const auto retreived_id_value = copy_int_from_buffer(hello_output, 14);
	BOOST_CHECK_EQUAL(retreived_id_value, id);
}


BOOST_AUTO_TEST_CASE(build_sample_hello_message_retreived_id_value_equals_2147483647)
{
	const auto id = 2147483647;
	const auto hello_output = create_hello_message("__input__.", id);
	const auto retreived_id_value = copy_int_from_buffer(hello_output, 14);
	BOOST_CHECK_EQUAL(retreived_id_value, id);
}

BOOST_AUTO_TEST_CASE(build_sample_hello_message_retreived_id_value_equals_minus_2147483648)
{
	const int32_t id = -2147483647 - 1; // no clue why can't explicitly write - 2147483648 and compile
	const auto hello_output = create_hello_message("__input__.", id);
	const auto retreived_id_value = copy_int_from_buffer(hello_output, 14);
	BOOST_CHECK_EQUAL(retreived_id_value, id);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(building_data_request)

BOOST_AUTO_TEST_CASE(build_data_request_contains_dot)
{
	data_request data_request_object;
	BOOST_CHECK_EQUAL(data_request_object.to_string(), std::string("."));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(building_data)

BOOST_AUTO_TEST_CASE(build_data_check_id_equals_1000)
{
	const auto id = 1000;
	data data_object(id);
	BOOST_CHECK_EQUAL(data_object.id(), id);
}

BOOST_AUTO_TEST_CASE(data_to_string_message_type_equals_send)
{
	data data_object(1);
	auto result = data_object.to_string();
	BOOST_CHECK_EQUAL(copy_int_from_buffer(result, 0), static_cast<int>(message_sent_type::data));
}

BOOST_AUTO_TEST_CASE(data_to_string_check_temperature_equals_100)
{
	const auto temperature = 100;
	data data_object(1);
	data_object.set_temperature(temperature);
	auto result = data_object.to_string();
	BOOST_CHECK_EQUAL(copy_int_from_buffer(result, 8), temperature);
}

BOOST_AUTO_TEST_CASE(data_to_string_check_id_equals_1000)
{
	const auto id = 1000;
	data data_object(id);
	auto result = data_object.to_string();
	BOOST_CHECK_EQUAL(copy_int_from_buffer(result, 4), id);
}

BOOST_AUTO_TEST_CASE(data_to_string_check_timestamp_equals_1000)
{
	const uint64_t timestamp = 1000;
	data data_object(1);
	data_object.set_timestamp(timestamp);
	auto result = data_object.to_string();
	BOOST_CHECK_EQUAL(copy_unsigned_long_from_buffer(result, 12), timestamp);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(building_challenge_reply)

BOOST_AUTO_TEST_CASE(challenge_reply_to_string_check_message_type_equals_challenge_reply)
{
	challenge_reply challenge_reply_object(std::string("1234567890123456", 16));
	const auto result = challenge_reply_object.to_string();
	BOOST_CHECK_EQUAL(copy_int_from_buffer(result, 0), static_cast<int>(message_sent_type::challenge_reply));
}

//
//BOOST_AUTO_TEST_CASE(challenge_reply_to_string_check_reply_equals_challenge_md5)
//{
//	const auto challenge = std::string("1234567890123456", 16);
//	challenge_reply challenge_reply_object(challenge);
//	const auto result = challenge_reply_object.to_string();
//	BOOST_CHECK_EQUAL(copy_string_from_buffer(result, 8, 16), calculate_md5(challenge));
//}


BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(build_access_denied_message)



BOOST_AUTO_TEST_SUITE_END()