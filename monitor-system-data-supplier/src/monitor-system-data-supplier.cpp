// monitor-system-data-supplier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "tcp_socket_controller.h"
#include "protocol_controller.h"

//TODO clear imports


std::string parse_input_arguments(int argc, char **argv)
{

	if (argc != 2) {
		throw std::runtime_error(std::string("usage: ") + std::string(argv[0]) + std::string(" <configuration_file_name>"));
	}
	return std::string(argv[1]);
}

protocol_controller create_controller(int argc, char **argv)
{
	protocol_controller controller;
	if (argc == 2)
	{
		const auto configuration_file_path = std::move(parse_input_arguments(argc, argv));
		controller.set_configuration(configuration_file_path);
	}
	else
	{
		controller.set_configuration();
	}
	return controller;
}

int main(int argc, char **argv)
{
	while(true)
	{
		try
		{
			std::cout << "Starting connection." << std::endl << std::endl;
			auto controller = std::move(create_controller(argc, argv));
			controller.establish_connection();
		}
		catch (std::runtime_error& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch (std::exception&)
		{
			std::cout << " An error occured. Rebooting app" << std::endl << std::endl;
		}
		boost::this_thread::sleep_for(boost::chrono::seconds{ 1 });
	}
	
	return 0;
}

