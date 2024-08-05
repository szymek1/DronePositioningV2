#include "../include/TelemetryReceiver.h"


TelemetryReceiver::TelemetryReceiver(EventsBus &bus, const std::string &portCom,
                                     bool isVerbose) 
    : m_portCom(portCom), m_verbose(isVerbose) { 
  m_publisher = bus.getPublisher();
  m_running.store(false);
  if (m_verbose) {
    std::cout << "TelemetryReceiver: instanitated" << std::endl;
  }
}

void TelemetryReceiver::receive_() {
	// Launching Processor thread
    m_running.store(true);
	if (m_verbose) {
		std::cout << "TelemetryReceiver: running receiver thread" << std::endl;
	}

    m_currTelemetry = {50.4, 20.4, 30.99};
	while (m_running.load()) { 
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
	if (m_verbose) {
		std::cout << "TelemetryReceiver: terminating..." << std::endl;
    }
	m_running.store(false);
}

void TelemetryReceiver::registerTelemetryEvent_() { 
	TelemetryEvent telemetry(m_currTelemetry);
	m_publisher->publish(EventType::TELEMETRY_UPDATE, telemetry); 
}
