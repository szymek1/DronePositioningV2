#pragma once

#include <string>
#include <memory>

#include "FlightConfig.h"

namespace configuration {

/**
 * @class ConfigurationManager
 * @brief This class reads and parses the flight configuration file
 */
class ConfigurationManager
{
public:
	ConfigurationManager() = default;
	~ConfigurationManager() = default;

	/**
	 * @brief Load the configuration file
	 * @param configFilePath The path to the configuration file
	 * @return A unique pointer to the FlightConfig object;
	 *		   MainController will take ownership of this object
	 */
	static std::unique_ptr<FlightConfig> loadConfig(const std::string& configFilePath);
};

} // namespace configuration

