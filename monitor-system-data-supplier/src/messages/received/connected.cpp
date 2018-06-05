#include "stdafx.h"
#include "connected.h"
#include "protocol_controller.h"

void connected::consume(protocol_controller& controller)
{
	controller.consume(this);
}
