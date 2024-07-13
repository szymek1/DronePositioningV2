#include "../include/TelemetryReceiver.h"


TelemetryReceiver::TelemetryReceiver(EventsBus& bus) {
  m_publisher = bus.getPublisher();
  m_telemetryProcessor = std::make_unique<IProcessor>(bus);
}

void TelemetryReceiver::receive() {
	// Dummy implementation for now
	
}
