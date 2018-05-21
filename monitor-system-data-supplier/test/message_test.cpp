#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "hello.h"
#include <iostream>
#include <Winsock2.h>

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

uint64_t copy_unsigned_long_from_buffer(const std::string& str, const int position)
{
	uint64_t_converter converter;
	memcpy(converter.char_array, str.c_str() + position, 8);
	return ntohll(converter.long_value);
}

BOOST_AUTO_TEST_SUITE(message_build_suite)

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