#include "ITelemetryReceiver.h"


void ITelemetryReceiver::receive() { 
	receive_(); 
}

void ITelemetryReceiver::stop() { 
	stop_(); 
}