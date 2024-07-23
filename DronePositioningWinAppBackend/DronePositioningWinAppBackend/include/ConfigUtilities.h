#pragma once

#include <string>
#include <stdexcept>
#include <regex>


/**
 * @brief Strucute defining a point which can be used to represent:
 * - Operator position
 * - Waypoint
 * - Marker
 */
struct Point {

	Point(double lat, double lon, double alt) : latitude(lat), longitude(lon), altitude(alt) {}

	double latitude;
	double longitude;
	double altitude;
};

/**
 * @brief Structure defining a position of the operator.
 */
struct OperatorPosition : public Point {
	OperatorPosition(double latitude, double longitude, double altitude)
		: Point(latitude, longitude, altitude) {}
	OperatorPosition() : Point(0, 0, 0) {}
};

/**
 * @brief Structure defining a waypoint: 
 *		  a point in the space that the drone has to reach.
 */
struct Waypoint : public Point {
	Waypoint(double latitude, double longitude, double altitude)
		: Point(latitude, longitude, altitude) {}
};

/**
 * @brief Structure defining a marker: 
 *		  a white circle placed on the ground level.
 */
struct Marker : public Point {

	Marker(double latitude, double longitude, double altitude, double radius)
		: Point(latitude, longitude, altitude) {
		if (radius < 0) {
			throw std::invalid_argument("Marker radius cannot be negative.");
		}
		this->radius = radius;
	}

	double radius;
};

/**
 * @brief Structure defining an obstackle:
 *		  a cuboid object that has to be avoided by the drone
 * - centerX, centerY, centerZ represent positon
 * - width, lenght, height represent dimensions
 * - r1, r2, r3 represent rotation angles
 */
struct Obstackle {

	Obstackle(double centerX, double centerY, double centerZ, double width, double lenght, double height, double rX, double rY, double rZ)
		: centerX(centerX), centerY(centerY), centerZ(centerZ), rX(rX), rY(rY), rZ(rZ) {
		if (width < 0 || lenght < 0 || height < 0) {
			throw std::invalid_argument("Obstackle dimensions cannot be negative.");
		}
		this->width = width;
		this->lenght = lenght;
		this->height = height;
	}

	double centerX;
	double centerY;
	double centerZ;
	double width;
	double lenght;
	double height;
	double rX;
	double rY;
	double rZ;
};

/***********************************************
* ENUMS defining less deterministic than 0 or 1
* values to be extracted from the configuration
* file
************************************************/
enum class CoorindatesSystem {
	GPS, // 0
	UCS //  1
};

enum class AltitudeDifference {
	DEFAULT, //  0
	IGNORE  //   1
};

enum class ScoringMethod {
	MAE,  // 0
	RMSE //  1
};

/**
 * @brief Structure defining an exercise.
 */
struct ExerciseInfo {
	std::string title;
	std::string author;
	CoorindatesSystem coordinatesSystem;
	AltitudeDifference altitudeDifference;
	float guidelineOffset;					// vertical offset value in meters, positive is up
	int distanceWeight;
	int altitudeWeight;
	int speedWeight;
	float targetSpeed;
	int accuracy;						    // distance to waypoint in meters to set next one as target 
	int pausingTime;						// hover time at waypoints, in seconds
	signed int targetBearings;				// UCS bearing in deg [0,360), -1 for next waypoint, -2 for none
	bool showWaypoints;
	bool showGuideline;
	ScoringMethod scoringMethod;
};

/**
 * @brief Structure defining a connection configuration.
 */
struct ConnectionConfigurationInfo {

	ConnectionConfigurationInfo(std::string remoteIp, int port) {
		if (!isValidPort(port) && !isValidIpAddress(remoteIp)) {
			throw std::invalid_argument("Invalid port or IP address.");
		}
		this->remoteIp = remoteIp;
	    this->port = port;
	}
	ConnectionConfigurationInfo() : remoteIp("0.0.0.0"), port(60000) {}

	std::string remoteIp;
	int port;

private:
	inline bool isValidPort(int port) const {
		return (port >= 1024 && port <= 49151) || (port >= 49152 && port <= 65535);
	}

	inline bool isValidIpAddress(const std::string& ipAddress) const {
		const std::regex pattern(
			R"(^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)"
		);
		return std::regex_match(ipAddress, pattern);
	}
};
