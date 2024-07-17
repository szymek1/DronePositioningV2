#include "ITelemetrySender.h"


void ITelemetrySender::sendPosition(const std::vector<float> &telemetry) { 
	sendPosition_(telemetry); 
}