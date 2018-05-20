// monitor-system-data-supplier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Winsock2.h>
#include "socket_controller.h"
#include <vector>
#include <boost/algorithm/string.hpp>

//TODO clear imports

std::vector<std::string> parse_input_arguments(int argc, char **argv)
{
	using namespace std;
	if (argc != 2) {
		throw runtime_error(string("usage: ")+ string(argv[0]) + string(" server-name:port"));
	}
	string sample(argv[1]);
	vector<string> strs;
	boost::split(strs, sample, boost::is_any_of(":"));
	if(strs.size() != 2)
	{
		throw runtime_error(string("usage: ") + string(argv[0]) + string(" server-name:port"));
	}
	return strs;
}


int main(int argc, char **argv)
{

	try
	{
		std::vector<std::string> arguments = std::move(parse_input_arguments(argc, argv));
		socket_controller controller(arguments[0], arguments[1]);
		controller.connect1();	
	}
	catch(std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch(std::exception& e)
	{
		std::cout << " An error occured ;)" << std::endl;
	}
	return 0;
}

