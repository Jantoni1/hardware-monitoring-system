#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "message_received_factory.h"
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


std::string int_to_char_pointer(const int32_t value)
{
	int32_t_converter converter;
	converter.int_value = htonl(value);
	char tab[4];
	memcpy(tab, converter.char_array, 4);
	return std::string(tab, 4);
}

void add_to_string(char* dest, const std::string& src, const int length)
{
	memcpy(dest, src.c_str(), length);
}

std::string create_hello_message_buffer(const std::string id, const std::string challenge)
{
	const auto message_type = message_received_type::hello_challenge;
	const auto message_type_string = int_to_char_pointer(static_cast<int>(message_type));
	char tab[24];
	memcpy(tab, message_type_string.c_str(), 4);
	add_to_string(tab + 4, id, 4);
	add_to_string(tab + 8, challenge, 16);
	return std::string(tab, 24);
}

BOOST_AUTO_TEST_SUITE(message_factory_test)

BOOST_AUTO_TEST_CASE(message_factory_parse_connected_message)
{
	message_received_factory message_factory;
	const auto message_type = message_received_type::connected;
	const auto message_type_string = int_to_char_pointer(static_cast<int>(message_type));
	auto *message = message_factory.build_message(message_type_string.c_str(), 4);
	BOOST_TEST(dynamic_cast<connected *>(message) != nullptr);
	delete message;
}

BOOST_AUTO_TEST_CASE(message_factory_parse_access_denied_message)
{
	message_received_factory message_factory;
	const auto message_type = message_received_type::access_denied;
	const auto message_type_string = int_to_char_pointer(static_cast<int>(message_type));
	auto *message = message_factory.build_message(message_type_string.c_str(), 4);
	BOOST_TEST(dynamic_cast<access_denied *>(message) != nullptr);
	delete message;
}

BOOST_AUTO_TEST_CASE(message_factory_parse_hello_challenge_message)
{
	message_received_factory message_factory;
	const auto message_type = message_received_type::hello_challenge;
	const auto message_type_string = int_to_char_pointer(static_cast<int>(message_type));
	char tab[24];
	memcpy(tab, message_type_string.c_str(), 4);
	auto *message = message_factory.build_message(tab , 24);
	BOOST_TEST(dynamic_cast<hello_challenge *>(message) != nullptr);
	delete message;
}

BOOST_AUTO_TEST_CASE(message_factory_parse_id_equals_15_and_challenge_equals_1234567890123456)
{
	message_received_factory message_factory;
	const auto id = 15;
	const auto challenge = std::string("1234567890123456", 16);
	const auto value = std::move(create_hello_message_buffer(int_to_char_pointer(id), challenge));
	auto *message = dynamic_cast<hello_challenge *>(message_factory.build_message(value.c_str(), value.size()));
	BOOST_CHECK_EQUAL(message->challenge(), challenge);
	BOOST_CHECK_EQUAL(message->id(), id);
	delete message;
}

BOOST_AUTO_TEST_CASE(message_factory_parse_message_with_message_type_equal_to_15_throw_exception)
{
	message_received_factory message_factory;
	const auto message_type = 15;
	const auto message_type_string = int_to_char_pointer(message_type);
	BOOST_CHECK_THROW(message_factory.build_message(message_type_string.c_str(), 4), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(message_factory_parse_message_with_message_type_equal_to_minus_1_throw_exception)
{
	message_received_factory message_factory;
	const auto message_type = -1;
	const auto message_type_string = int_to_char_pointer(message_type);
	BOOST_CHECK_THROW(message_factory.build_message(message_type_string.c_str(), 4), std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END()

