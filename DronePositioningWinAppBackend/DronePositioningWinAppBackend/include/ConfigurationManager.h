/**
 * @file ConfigurationManager.h
 * @brief Configuration manager.
 *
 * @details This file contains declaration for ConfigurationManager.
 *
 * @author Szymon Bogus
 * @date 2024-05-26
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 *
 * @version 1.0
 */

#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <filesystem>
#include <fstream>
#include <exception>
#include <string_view>

#include "FlightConfig.h"


namespace configuration {

/**
 * @class ConfigurationManager
 * @brief This class reads and parses the flight configuration file.
 */
class ConfigurationManager
{
public:
	ConfigurationManager() = default;
	~ConfigurationManager() = default;

	/**
	 * @brief Load the configuration file
	 * @param configFilePath: the path to the configuration file
	 * @return A unique pointer to the FlightConfig object;
	 *		   MainController will take ownership of this object
	 */
	static std::unique_ptr<FlightConfig> loadConfig(const std::filesystem::path& configFilePath);

};

} // namespace configuration

