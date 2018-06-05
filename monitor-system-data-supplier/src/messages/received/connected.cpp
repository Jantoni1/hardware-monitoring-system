
#include "connected.h"
#include "controllers/protocol_controller.h"

void connected::consume(protocol_controller& controller)
{
	controller.consume(this);
}
