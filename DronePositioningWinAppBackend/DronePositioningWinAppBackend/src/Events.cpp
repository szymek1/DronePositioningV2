#include "../include/Events.h"


TelemetryEvent::TelemetryEvent(std::vector<float> data) 
	: telemetry(data) {}

ConnectionEvent::ConnectionEvent(bool status, const std::string &who,
                                 const std::string &message)
    : isConnected(status), whichComponent(who), connMess(message) {}

AppTerminationEvent::AppTerminationEvent(bool status)
    : isAppTerminating(status) {}