#pragma once
#include "vcruntime_exception.h"
class EmptyDataBufferException :
	public std::exception
{
public:
	EmptyDataBufferException();
	~EmptyDataBufferException();
};

