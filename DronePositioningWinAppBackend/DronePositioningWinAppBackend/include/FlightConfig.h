/**
 * @file FlightConfig.h
 * @brief FLight config obejct representation.
 *
 * @details This file contains the declaration for object which represents parsed config data.
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
#define FMT_HEADER_ONLY

#include <string>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

#include "ConfigUtilities.h"


namespace configuration {

/**
* @class FlightConfig 
* @brief Class representing flight configuration for a specific
*		 training session.
*/
class FlightConfig
{
public:
	FlightConfig(OperatorPosition opPos,
				 std::vector<Waypoint> waypoints,
				 std::vector<Marker> markers,
				 std::vector<Obstackle> obstacles,
				 ExerciseInfo exerciseInfo,
				 ConnectionConfigurationInfo connectionInfo);
	~FlightConfig() = default;

	const OperatorPosition& getOperatorPosition() const;
	const std::vector<Waypoint>& getWaypoints() const;
	const std::vector<Marker>& getMarkers() const;
	const std::vector<Obstackle>& getObstacles() const;
	const ExerciseInfo& getExerciseInfo() const;
	const ConnectionConfigurationInfo& getConnectionConfigurationInfo() const;

	/**
	* @brief Present loaded configuration.
	*/
	void displayLoadedConfiguration() const;

private:
	
	/***********************************************
	* Objects to display during the flight
	************************************************/
	OperatorPosition m_operatorPosition;
	std::vector<Waypoint> m_waypoints;
	std::vector<Marker> m_markers;
	std::vector<Obstackle> m_obstacles;

	/***********************************************
	* Configuration of the flight
	***********************************************/
	ExerciseInfo m_exerciseInfo;
	ConnectionConfigurationInfo m_connectionConfigurationInfo;

};

} // namespace configuration

