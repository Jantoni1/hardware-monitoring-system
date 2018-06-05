#include "stdafx.h"
#include "hello_challenge.h"
#include "protocol_controller.h"

void hello_challenge::consume(protocol_controller& controller)
{
	controller.consume(this);
}
