// monitor-system-data-supplier.cpp : Defines the entry point for the console application.
//

#include "thread_controller.h"
#include <csignal>
#include <boost/asio.hpp>

int main(int argc, char **argv)
{
	thread_controller controller(argc, argv);
	try
	{
		controller.run_connection_threads();
		boost::asio::io_service ios;
		boost::asio::signal_set signals(ios, SIGINT, SIGTERM);
		signals.async_wait([&](boost::system::error_code const&, int) {
			std::cout << "STOPPING..." << std::endl;
			controller.join();
		});
		ios.run();
	}
	catch(std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "Unknown error." << std::endl;
	}
	return 0;
}

