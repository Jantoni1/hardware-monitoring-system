#include "stdafx.h"
#include "configuration.h"
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>

configuration::~configuration()
{
}

void configuration::parse_configuration()
{
	std::ifstream input_stream(configuration_file_path_);
	if(!input_stream.is_open()) {
		throw std::runtime_error("Could not open configuration file.");
	}
	std::string line, key, value;
	try {
		while (std::getline(input_stream >> std::ws, line) && !line.empty()) {
			parse_line(line);
		}
	}
	catch(std::runtime_error &e) {
		input_stream.close();
		throw e;
	}
	catch(boost::bad_lexical_cast &) {
		input_stream.close();
		throw std::runtime_error("Incorrect value in line: " + line);
	}
	input_stream.close();
	check_default_values_changed();
}

void configuration::set_tcp_server_ip_address(const std::string& tcp_server_ip_address)
{
	parse_to_ip_address(tcp_server_ip_address);
	tcp_server_ip_address_ = tcp_server_ip_address;
}

void configuration::set_tcp_server_port(const std::string& tcp_server_port)
{
	boost::lexical_cast<int>(tcp_server_port);
	tcp_server_port_ = tcp_server_port;
}

void configuration::set_tcp_daemon_ip_address(const std::string& tcp_daemon_ip_address)
{
	parse_to_ip_address(tcp_daemon_ip_address);
	tcp_daemon_ip_address_ = tcp_daemon_ip_address;
}

void configuration::set_tcp_daemon_port(const std::string& tcp_daemon_port)
{
	boost::lexical_cast<int>(tcp_daemon_port);
	tcp_daemon_port_ = tcp_daemon_port;
}

void configuration::set_udp_server_ip_address(const std::string& udp_server_ip_address)
{
	parse_to_ip_address(udp_server_ip_address);
	udp_server_ip_address_ = udp_server_ip_address;
}

void configuration::set_udp_server_port(const std::string& udp_server_port)
{
	boost::lexical_cast<int>(udp_server_port);
	udp_server_port_ = udp_server_port;
}

void configuration::set_identification_name(const std::string& identification_name)
{
	if(identification_name.size() > 10 || identification_name.empty()) {
		throw std::runtime_error("Identification name must be between 1 and 10 characters long.");
	}
	identification_name_ = identification_name;
}

void configuration::check_default_values_changed() const
{
	if(id_ == -1 || identification_name_ == "undefined")
	{
		throw std::runtime_error("Program requires at least id and identification_name in configuration file in order to run.");
	}
}

void configuration::parse_line(const std::string& line)
{
	std::string key, value;
	std::istringstream s_stream(line);
	s_stream >> key;
	boost::erase_all(key, ":");
	s_stream >> value;
	if (parser_map_.find(key) != parser_map_.end())
	{
		parser_map_.at(key)(value);
	}
	else
	{
		throw std::runtime_error("Found unknown tag: " + key + " during parsing configuration file.");
	}
}

void configuration::parse_to_ip_address(const std::string& ip) const
{
	std::vector<std::string> numbers;
	boost::split(numbers, ip, boost::is_any_of("."));
	if(numbers.size() != 4)
	{
		throw std::runtime_error("The following IP address: " + ip + "in configuration file is incorrect.");
	}
	for(auto& number : numbers)
	{
		const auto converted_number = boost::lexical_cast<int>(number);
		if(converted_number < 0 || converted_number > 255)
		{
			throw std::runtime_error("The following IP address: " + ip + "in configuration file is incorrect.");
		}
	}

}