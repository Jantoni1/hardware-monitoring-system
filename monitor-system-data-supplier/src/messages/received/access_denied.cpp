#include "stdafx.h"
#include <access_denied.h>
#include "protocol_controller.h"

void access_denied::consume(protocol_controller& controller)
{
	controller.consume(this);
}
