#pragma once
class socket_controller
{
public:
	socket_controller();
	~socket_controller();

	int connect1(int argc, char **argv);

	bool authorization_done;
};

