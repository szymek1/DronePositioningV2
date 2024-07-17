#include "../include/TelemetryReceiver.h"


TelemetryReceiver::TelemetryReceiver(EventsBus &bus,
                                     std::atomic_bool &appStatus)
	: m_isRunning(appStatus) {
  m_publisher = bus.getPublisher();
  m_telemetryProcessor = std::make_unique<IProcessor>(bus);
}

void TelemetryReceiver::receive_() {
	// Launching Processor thread
    m_currTelemetry = {50.4, 20.4, 30.99};
	while (m_isRunning.load()) {
		// Read data
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
		// Some condition to say when we have a valid new telemetry
        registerTelemetryEvent_();
	}

	// This section launches after stop_() shuts down the loop.
	// It's responsible for:
	// cleaning resources, calling Processor to generate the report, shutting down Processor thread
	
}

void TelemetryReceiver::stop_() { 
	m_isRunning.store(false); 
}

void TelemetryReceiver::registerTelemetryEvent_() { 
	TelemetryEvent telemetry(m_currTelemetry);
	m_publisher->publish(EventType::TELEMETRY_UPDATE, telemetry); 
}
