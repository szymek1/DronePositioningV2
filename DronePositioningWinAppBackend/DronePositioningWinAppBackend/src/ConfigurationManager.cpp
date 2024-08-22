/**
 * @file ConfigurationManager.cpp
 * @brief Configuration file parser code.
 *
 * @details This file contains the definition of configuration file parser.
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

#include "../include/ConfigurationManager.h"


namespace configuration {

	std::unique_ptr<FlightConfig> ConfigurationManager::loadConfig(const std::filesystem::path& configFilePath)
	{
		if (!std::filesystem::exists(configFilePath))
		{
			throw std::runtime_error("File not found: " + configFilePath.string());
		}

		/***********************************************
		* Initializing vectors and objects to hold 
		* particular part of the configuration file
		************************************************/
		OperatorPosition operatorPosition;
		std::vector<Waypoint> waypoints;
		std::vector<Marker> markers;
		std::vector<Obstackle> obstacles;
		ExerciseInfo exerciseInfo;
		ConnectionConfigurationInfo connectionInfo;

		std::ifstream file(configFilePath);
		std::string line;
		std::string currentSection;

		while (std::getline(file, line)) {
			// Ignore comments and empty lines
			if (line.empty() || line[0] == '#') {
				continue;
			}

			// Check for section headers (e.g., "Waypoints:")
			if (line.back() == ':') {
				currentSection = line.substr(0, line.size() - 1); // Remove the trailing ':'
				continue;
			}

            if (line == "File End") {
                break; // Break iterating and move to FlighConfig instantiation
            }

			// Process data based on the current section
            if (currentSection == "Title") {
                exerciseInfo.title = line;
            } else if (currentSection == "Author") {
                exerciseInfo.author = line;
            } else if (currentSection == "Operator position") {
                std::istringstream iss(line);
                double lat, lon, alt;
                if (iss >> lat >> lon >> alt) {
                    operatorPosition = OperatorPosition(lat, lon, alt);
                }
                else {
                    throw std::runtime_error("Invalid operator position format: " + line);
                }
            } else if (currentSection == "Waypoints") {
                std::istringstream iss(line);
                double x, y, z; // Depending on the coordinate system, this could be lat, lon, alt
                if (iss >> x >> y >> z) {
                    waypoints.emplace_back(x, y, z);
                }
                else {
                    throw std::runtime_error("Invalid waypoint format: " + line);
                }
            } else if (currentSection == "Markers") {
                std::istringstream iss(line);
                double x, y, z, radius; // Depending on the coordinate system, this could be lat, lon, alt + radius
                if (iss >> x >> y >> z >> radius) {
                    markers.emplace_back(x, y, z, radius);
                }
                else {
                    throw std::runtime_error("Invalid marker format: " + line);
                }
            } else if (currentSection == "Obstacles") {
                std::istringstream iss(line);
                double centerX, centerY, centerZ, width, length, height, rotX, rotY, rotZ;
                if (iss >> centerX >> centerY >> centerZ >> width >> length >> height >> rotX >> rotY >> rotZ) {
                    obstacles.emplace_back(centerX, centerY, centerZ, width, length, height, rotX, rotY, rotZ);
                }
                else {
                    throw std::runtime_error("Invalid obstackle format: " + line);
                }
            } else if (currentSection == "CoordsInGps") {
                std::istringstream iss(line);
                int coordsInGps;
                if (iss >> coordsInGps) {
                    exerciseInfo.coordinatesSystem = static_cast<CoorindatesSystem>(coordsInGps);
                }
                else {
                    throw std::runtime_error("Invalid coordsInGps format: " + line);
                }
            } else if (currentSection == "AltitudeDiffIgnore") {
                std::istringstream iss(line);
                int altitudeDifference;
                if (iss >> altitudeDifference) {
                    exerciseInfo.altitudeDifference = static_cast<AltitudeDifference>(altitudeDifference);
                }
                else {
                    throw std::runtime_error("Invalid altitudeDifference format: " + line);
                }
            } else if (currentSection == "GuidelineOffset") {
                std::istringstream iss(line);
                float guidelineOffset;
                if (iss >> guidelineOffset) {
                    exerciseInfo.guidelineOffset = guidelineOffset;
                }
                else {
                    throw std::runtime_error("Invalid guidelineOffset format: " + line);
                }
            } else if (currentSection == "DistanceWeight") {
                std::istringstream iss(line);
                int distanceWeight;
                if (iss >> distanceWeight) {
                    exerciseInfo.distanceWeight = distanceWeight;
                }
                else {
                    throw std::runtime_error("Invalid distanceWeight format: " + line);
                }
            } else if (currentSection == "AltitudeWeight") {
                std::istringstream iss(line);
                int attitudeWeight;
                if (iss >> attitudeWeight) {
                    exerciseInfo.altitudeWeight = attitudeWeight;
                }
                else {
                    throw std::runtime_error("Invalid attitudeWeight format: " + line);
                }
            } else if (currentSection == "SpeedWeight") {
                std::istringstream iss(line);
                int speedWeight;
                if (iss >> speedWeight) {
                    exerciseInfo.speedWeight = speedWeight;
                }
                else {
                    throw std::runtime_error("Invalid speedWeight format: " + line);
                }  
            } else if (currentSection == "TargetSpeed") {
                std::istringstream iss(line);
                float targetSpeed;
                if (iss >> targetSpeed) {
                    exerciseInfo.targetSpeed = targetSpeed;
                }
                else {
                    throw std::runtime_error("Invalid targetSpeed format: " + line);
                }
            }
            else if (currentSection == "Accuracy") {
                std::istringstream iss(line);
                int accuracy;
                if (iss >> accuracy) {
                    exerciseInfo.accuracy = accuracy;
                }
                else {
                    throw std::runtime_error("Invalid accuracy format: " + line);
                }
            } else if (currentSection == "PausingTime") {
                std::istringstream iss(line);
                int pausingTime;
                if (iss >> pausingTime) {
                    exerciseInfo.pausingTime = pausingTime;
                }
                else {
                    throw std::runtime_error("Invalid pausingTime format: " + line);
                }
            } else if (currentSection == "TargetBearing") {
                std::istringstream iss(line);
                signed int targetBearing;
                if (iss >> targetBearing) {
                    exerciseInfo.targetBearings = targetBearing;
                }
                else {
                    throw std::runtime_error("Invalid targetBearing format: " + line);
                }
            }  else if (currentSection == "ShowWaypoints") {
                std::istringstream iss(line);
                int showWaypoints;
                if (iss >> showWaypoints) {
                    exerciseInfo.showWaypoints = static_cast<bool>(showWaypoints);
                }
                else {
                    throw std::runtime_error("Invalid showWaypoints format: " + line);
                }
            } else if (currentSection == "ShowGuideline") {
                std::istringstream iss(line);
                int showGuideline;
                if (iss >> showGuideline) {
                    exerciseInfo.showGuideline = static_cast<bool>(showGuideline);
                }
                else {
                    throw std::runtime_error("Invalid showGuideline format: " + line);
                }
            } else if (currentSection == "ScoringMethod") {
                std::istringstream iss(line);
                int scoringMethod;
                if (iss >> scoringMethod) {
                    exerciseInfo.scoringMethod = static_cast<ScoringMethod>(scoringMethod);
                }
                else {
                    throw std::runtime_error("Invalid scoringMethod format: " + line);
                }
            } else if (currentSection == "ConnectionInfo") {
                std::istringstream iss(line);
                std::string remoteIp;
                int port;
                if (iss >> remoteIp >> port) {
                    connectionInfo = ConnectionConfigurationInfo(remoteIp, port);
                }
                else {
                    throw std::runtime_error("Invalid connectionInfo format: " + line);
                }
            }
		}

        file.close();

        std::unique_ptr<FlightConfig> flConf = std::make_unique<FlightConfig>(
            std::move(operatorPosition),
            std::move(waypoints),
            std::move(markers),
            std::move(obstacles),
            std::move(exerciseInfo),
            std::move(connectionInfo)
        );

		return flConf;
	}

} // namespace configuration
