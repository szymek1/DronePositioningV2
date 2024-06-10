#include "../include/ConfigurationManager.h"


namespace configuration {

	std::unique_ptr<FlightConfig> ConfigurationManager::loadConfig(const std::filesystem::path& configFilePath)
	{
		if (!std::filesystem::exists(configFilePath))
		{
			throw std::runtime_error("File not found: " + configFilePath.string());
		}
		return std::unique_ptr<FlightConfig>();
	}

} // namespace configuration
