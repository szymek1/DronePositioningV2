#include "../include/MainController.h"

MainController::MainController(const std::filesystem::path &flightConfigPath, EventsBus& bus,
                               bool isVerbose) : m_bus(bus), m_verbose(isVerbose) {
	if (initialize_(flightConfigPath)) {
		std::cout << "Configuration loaded" << std::endl;
        m_publisher = m_bus.getPublisher();

		m_isRunning.store(false);

        if (isVerbose) {
            m_flightConfig->displayLoadedConfiguration();
        }
    } else {
        std::cout << "Couldnt load the configuration" << std::endl;
	}

	
	// TODO: prepare run method: focus on threading and proper shutdown of them, next move one to proper callback
	//		 with EventBus
}

void MainController::run() {
	m_isRunning.store(true);
    std::cout << "Running MainController" << std::endl;

	m_telemetryProcessor = std::make_shared<TelemetryProcessor>();
    m_telemetryReceiver = std::make_shared<TelemetryReceiver>(
        m_bus, m_telemetryProcessor, m_isRunning);
    m_telemetrySender = std::make_shared<TelemetrySender>();

	m_bus.addSubscriber(EventType::TELEMETRY_UPDATE, m_telemetrySender);
    m_bus.addSubscriber(EventType::TELEMETRY_UPDATE, m_telemetryProcessor);

    // TODO: launch threads etc...
    while (m_isRunning.load()) {
        // Main loop
    }
    std::cout << "Shutting down..." << std::endl;
}

bool MainController::shutdown() { 
	m_isRunning.store(false);

	// Announce shutdown
    AppTerminationEvent terminationEvent(true);
    m_publisher->publish(EventType::APP_TERMINATION, terminationEvent);

	return true; 
}

bool MainController::initialize_(const std::filesystem::path& flightConfigPath)
{
	try {
		m_flightConfig = configuration::ConfigurationManager::loadConfig(flightConfigPath);
		return true;
	}
	catch (const std::runtime_error& configuration_load_err) {
		std::cout << configuration_load_err.what() << std::endl;
		return false;
	}
}
