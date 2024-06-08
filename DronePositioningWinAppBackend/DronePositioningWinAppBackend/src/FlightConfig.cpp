#include "../include/FlightConfig.h"

configuration::FlightConfig::FlightConfig(OperatorPosition opPos,
										  std::vector<Waypoint> waypoints,
										  std::vector<Marker> markers,
										  std::vector<Obstackle> obstacles,
										  ExerciseInfo exerciseInfo,
										  ConnectionConfigurationInfo connectionInfo)
				: m_operatorPosition(std::move(opPos)), 
				m_waypoints(std::move(waypoints)),
				m_markers(std::move(markers)),
				m_obstacles(std::move(obstacles)),
				m_exerciseInfo(std::move(exerciseInfo)),
				m_connectionConfigurationInfo(std::move(connectionInfo)) {}

const OperatorPosition& configuration::FlightConfig::getOperatorPosition() const
{
	return m_operatorPosition;
}

const std::vector<Waypoint>& configuration::FlightConfig::getWaypoints() const
{
	return m_waypoints;
}

const std::vector<Marker>& configuration::FlightConfig::getMarkers() const
{
	return m_markers;
}

const std::vector<Obstackle>& configuration::FlightConfig::getObstacles() const
{
	return m_obstacles;
}

const ExerciseInfo& configuration::FlightConfig::getExerciseInfo() const
{
	return m_exerciseInfo;
}

const ConnectionConfigurationInfo& configuration::FlightConfig::getConnectionConfigurationInfo() const
{
	return m_connectionConfigurationInfo;
}
