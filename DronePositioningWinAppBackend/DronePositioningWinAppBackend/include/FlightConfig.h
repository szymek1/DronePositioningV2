#pragma once

#include <string>
#include <vector>

#include "ConfigUtilities.h"

namespace configuration {

class FlightConfig
{
public:
	FlightConfig();
	~FlightConfig() = default;

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

