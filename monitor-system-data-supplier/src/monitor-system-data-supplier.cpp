// monitor-system-data-supplier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Winsock2.h>
#include "socket_controller.h"

//TODO clear imports

int main(int argc, char **argv)
{
	
	socket_controller controller;
	controller.connect1(argc, argv);
//
//	try {
//		if (true)
//		{
//			controller.connect1(argc, argv);
//		}
//	}
//	catch (std::exception& )
//	{
//		std::cout << argc << std::endl;
//	}
//	
    return 0;
}

