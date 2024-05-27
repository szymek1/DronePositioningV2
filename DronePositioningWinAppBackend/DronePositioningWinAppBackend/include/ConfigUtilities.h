#pragma once

#include <string>


/**
 * @brief Strucute defining a point which can be used to represent:
 * - Operator position
 * - Waypoint
 * - Marker
 */
struct Point {
	double latitude;
	double longitude;
	double altitude;
};

/**
 * @brief Structure defining a position of the operator
 */
struct OperatorPosition : public Point {};

/**
 * @brief Structure defining a waypoint: 
 *		  a point in the space that the drone has to reach
 */
struct Waypoint : public Point {};

/**
 * @brief Structure defining a marker: 
 *		  a white circle placed on the ground level
 */
struct Marker : public Point {
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
	double centerX;
	double centerY;
	double centerZ;
	double width;
	double lenght;
	double height;
	double r1;
	double r2;
	double r3;
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
	DEFAULT, // 0
	IGNORE //   1
};

enum class ScoringMethod {
	MAE, // 0
	RMSE // 1
};

/**
 * @brief Structure defining an exercise
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
	int targetBearings;						// UCS bearing in deg [0,360), -1 for next waypoint, -2 for none
	bool showWaypoints;
	bool showGuideline;
	ScoringMethod scoringMethod;
};

/**
 * @brief Structure defining a connection configuration
 */
struct ConnectionConfigurationInfo {
	std::string remoteIp;
	int port;
};
