// monitor-system-data-supplier.cpp : Defines the entry point for the console application.
//

#include "thread_controller.h"
#include <csignal>
#include <boost/asio.hpp>

int main(int argc, char **argv)
{
	
	try
	{
		auto *ios_ptr = new boost::asio::io_service();
		boost::asio::signal_set signals(*ios_ptr, SIGINT, SIGTERM);
		std::shared_ptr<boost::asio::io_service> ios(ios_ptr);
		thread_controller controller(argc, argv, ios);
		signals.async_wait([&](boost::system::error_code const&, int) {
			std::cout << "STOPPING..." << std::endl;
			controller.join();
		});
		controller.run_connection_threads();
		ios->run();
		if(ios->stopped())
		{
			controller.join();
		}
	}
	catch(std::runtime_error e)
	{
		std::cout << e.what();
	}
	catch (std::exception e)
	{
		std::cout << e.what();
	}
	catch(...)
	{
		std::cout << "Unknown error." << std::endl;
	}
	return 0;
}

