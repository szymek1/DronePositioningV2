#include "../include/MainController.h"

MainController::MainController(const std::filesystem::path &flightConfigPath,
                               EventsBus &bus, bool isVerbose) {
	if (initialize_(flightConfigPath)) {
		std::cout << "Configuration loaded" << std::endl;
        m_publisher = bus.getPublisher();

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
