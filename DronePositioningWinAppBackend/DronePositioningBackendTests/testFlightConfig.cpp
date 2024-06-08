#include <string>

#include <gtest/gtest.h>

#include "../DronePositioningWinAppBackend/include/FlightConfig.h"


using namespace configuration;


std::vector<Waypoint> test_waypoints = {
	{12.0, 4.0, 0.0},
	{17.0, -4.0, 0.4 },
	{-0.123, 4.5, 0.2},
};

std::vector<Marker> test_markers = {
	{12.0, 4.0, 0.0, 10.0},
	{17.0, -4.0, 0.4, 2.5},
	{-0.123, 4.5, 0.2, 3.0},
};

std::vector<Obstackle> test_obstaclkes = {
	{12.0, 4.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0},
	{17.0, -4.0, 0.4, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0},
	{-0.123, 4.5, 0.2, 3.0, 3.0, 3.0, 0.0, 0.0, 0.0},
};

std::vector< ConnectionConfigurationInfo> test_connectionsInfo = {
	{"0.0.0.0", 4000},
	{"256.12.34.56", 50100},
	{"192.168.1.100", 60001}
};

std::vector< ConnectionConfigurationInfo> incorrect_test_connectionsInfo = {
	{"0.0.0.", 4000},
	{"256.12", 50100},
	{"192.168.1.100", 6},
	{"192.168.1.100", 70000}
};

