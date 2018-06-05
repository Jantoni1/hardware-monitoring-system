#pragma once
#include "protocol_controller.h"

class thread_controller
{
public:
	thread_controller(int number_of_arguments, char **array);
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
};

