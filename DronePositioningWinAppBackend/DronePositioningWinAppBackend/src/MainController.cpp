#include "../include/MainController.h"

MainController::MainController(const std::filesystem::path& flightConfigPath, bool isVerbose)
{
	if (initialize_(flightConfigPath)) {
		std::cout << "Configuration loaded" << std::endl;
        if (isVerbose) {
            m_flightConfig->displayLoadedConfiguration();
        }
    } else {
        std::cout << "Couldnt load the configuration" << std::endl;
	}

	
	// TODO: prepare run method: focus on threading and proper shutdown of them, next move one to proper callback
	//		 with EventBus
}

void MainController::run() {}

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
