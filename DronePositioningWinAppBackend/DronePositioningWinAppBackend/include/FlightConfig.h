#pragma once
#define FMT_HEADER_ONLY

#include <string>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

#include "ConfigUtilities.h"


namespace configuration {

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
	* @brief Present loaded configuration
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

