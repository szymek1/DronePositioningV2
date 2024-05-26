#pragma once

#include <string>

#include "FlightConfig.h"

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
	 */
	static void loadConfig(const std::string& configFilePath, FlightConfig& flightConfig);
};

