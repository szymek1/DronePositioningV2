/**
 * @file FlightConfig.cpp
 * @brief FlightConfig and configuration display code.
 *
 * @details This file contains the declaration for object which represents the flight config.
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

void configuration::FlightConfig::displayLoadedConfiguration() const {
    fmt::print("\n=== Flight Configuration ===\n");
    fmt::print("Exercise:\n");
    fmt::print("  Title:           {}\n", m_exerciseInfo.title);
    fmt::print("  Author:          {}\n", m_exerciseInfo.author);
    fmt::print("  Coordinates System: {}\n",
             m_exerciseInfo.coordinatesSystem == CoorindatesSystem::GPS
                 ? "GPS"
                 : "UCS");
    fmt::print("  Altitude Difference: {}\n",
             m_exerciseInfo.altitudeDifference == AltitudeDifference::DEFAULT
                 ? "Default"
                 : "Ignore");
    fmt::print("  guidelineOffset: {}\n", m_exerciseInfo.guidelineOffset);
    fmt::print("  distanceWeight:  {}\n", m_exerciseInfo.distanceWeight);
    fmt::print("  altitudeWeight:  {}\n", m_exerciseInfo.altitudeWeight);
    fmt::print("  speedWeight:     {}\n", m_exerciseInfo.speedWeight);
    fmt::print("  targetSpeed:     {}\n", m_exerciseInfo.targetSpeed);
    fmt::print("  accuracy:        {}\n", m_exerciseInfo.accuracy);
    fmt::print("  pausingTime:     {}\n", m_exerciseInfo.pausingTime);
    fmt::print("  targetBearings:  {}\n", m_exerciseInfo.targetBearings);
    fmt::print("  showWaypoints:   {}\n", m_exerciseInfo.showWaypoints);
    fmt::print("  showGuideline:   {}\n", m_exerciseInfo.showGuideline);
    fmt::print("  Scoring Method:  {}\n",
               m_exerciseInfo.scoringMethod == ScoringMethod::MAE
                   ? "MAE"
                   : "RMSE");

    fmt::print("\nConnection:\n");
    fmt::print("  Remote IP:       {}\n", m_connectionConfigurationInfo.remoteIp);
    fmt::print("  Port:            {}\n", m_connectionConfigurationInfo.port);

    fmt::print("\nOperator Position:\n");
    fmt::print("  Latitude:        {}\n", m_operatorPosition.latitude);
    fmt::print("  Longitude:       {}\n", m_operatorPosition.longitude);
    fmt::print("  Altitude:        {}\n", m_operatorPosition.altitude);

    fmt::print("\nWaypoints ({}):\n", m_waypoints.size());
    for (const auto& wp : m_waypoints) {
      fmt::print("  ({}, {}, {})\n", wp.latitude, wp.longitude, wp.altitude);
    }

    fmt::print("\nMarkers ({}):\n", m_markers.size());
    for (const auto& marker : m_markers) {
      fmt::print("  ({}, {}, {}) - Radius: {}\n", marker.latitude,
               marker.longitude, marker.altitude, marker.radius);
    }

    fmt::print("\nObstacles ({}):\n", m_obstacles.size());
    for (const auto& obst : m_obstacles) {
      fmt::print("  Center: ({}, {}, {})\n", obst.centerX, obst.centerY,
               obst.centerZ);
      fmt::print("  Dimensions: ({}, {}, {})\n", obst.width, obst.lenght,
               obst.height);
      fmt::print("  Rotation: ({}, {}, {})\n", obst.rX, obst.rY, obst.rZ);
    }
}
