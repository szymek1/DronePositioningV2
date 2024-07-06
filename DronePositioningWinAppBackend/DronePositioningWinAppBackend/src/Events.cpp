#include "../include/Events.h"


TelemetryEvent::TelemetryEvent(std::vector<float> data) 
	: telemetry(data) {}

ConnectionEvent::ConnectionEvent(bool status, const std::string &who)
    : isConnected(status), whichComponent(who) {}

AppTerminationEvent::AppTerminationEvent(bool status)
    : isAppTerminating(status) {}