#pragma once
#include "protocol_controller.h"
#include <boost/asio/io_service.hpp>

class thread_controller
{
public:
	thread_controller(const int number_of_arguments, char** array, std::shared_ptr<boost::asio::io_service> &ios);
	~thread_controller();

	void run_connection_threads();
	void join();
	
private:
	void set_configuration();
	void run_threads();

	int number_of_arguments_;
	char** array_;
	protocol_controller protocol_controller_;

	boost::thread *connection_thread_;

	std::shared_ptr<boost::asio::io_service> ios_;
};

