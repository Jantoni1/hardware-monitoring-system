#pragma once
#include "vcruntime_exception.h"
class empty_data_buffer_exception :
	public std::exception
{
public:
	empty_data_buffer_exception();
	~empty_data_buffer_exception();
};

