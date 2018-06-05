#include "stdafx.h"
#include "thread_controller.h"
#include <csignal>


thread_controller::thread_controller(const int number_of_arguments, char** array)
	: number_of_arguments_(number_of_arguments)
	, array_(array)
{

}

thread_controller::~thread_controller()
{
	delete connection_thread_;
}

void thread_controller::set_configuration()
{
		if (number_of_arguments_ != 2) {
			throw std::runtime_error(std::string("usage: ") + std::string(array_[0]) + std::string(" <configuration_file_name>"));
		}
		return protocol_controller_.set_configuration(std::move(std::string(array_[1])));
}

void thread_controller::run_threads()
{
	try
	{
		while (true)
		{
			try
			{
				std::cout << "Starting connection." << std::endl << std::endl;
				protocol_controller_.establish_connection();
			}
			catch (std::runtime_error& e)
			{
				std::cout << e.what() << std::endl;
			}
			catch (std::exception &e)
			{
				if(protocol_controller_.current_stage() == message_received_type::access_denied)
				{
					std::raise(SIGINT);
				}
				std::cout << e.what() << std::endl;
				std::cout << " An error occured. Rebooting app" << std::endl << std::endl;
			}
			boost::this_thread::sleep_for(boost::chrono::seconds{ 1 });
		}
	}
	catch(boost::thread_interrupted &e) {
	}
	protocol_controller_.shut_down_threads();

}

void thread_controller::run_connection_threads()
{
	set_configuration();
	connection_thread_ = new boost::thread{ boost::bind(&thread_controller::run_threads, this) };
}

void thread_controller::join()
{
	connection_thread_->interrupt();
	connection_thread_->join();
}
