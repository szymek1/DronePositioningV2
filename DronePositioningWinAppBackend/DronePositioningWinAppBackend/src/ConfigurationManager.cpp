#include "../include/ConfigurationManager.h"


namespace configuration {

	std::unique_ptr<FlightConfig> ConfigurationManager::loadConfig(const std::string& configFilePath)
	{
		return std::unique_ptr<FlightConfig>();
	}

} // namespace configuration
