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

/**
 * @brief Structure defining an exercise
 */
struct ExerciseInfo {
	std::string title;
	std::string author;
};

struct ConnectionConfigurationInfo {
	std::string ip;
	int port;
};

