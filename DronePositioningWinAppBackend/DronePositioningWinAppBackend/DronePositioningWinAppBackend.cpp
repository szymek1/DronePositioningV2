#include <iostream>

#include "include/FlightConfig.h"


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

std::vector<ConnectionConfigurationInfo> incorrect_test_connectionsInfo = {
   {"0.0.0.", 4000},
   {"256.12", 50100},
   {"192.168.1.100", 6},
   {"192.168.1.100", 70000}
};

int main()
{
    OperatorPosition test_operatorPosition = OperatorPosition(12.0, 13.0, 15.0);
	ExerciseInfo test_exerciseInfo = ExerciseInfo();

	test_exerciseInfo.title = "title";
	test_exerciseInfo.author = "author";
	test_exerciseInfo.coordinatesSystem = CoorindatesSystem::UCS;
	test_exerciseInfo.altitudeDifference = AltitudeDifference::DEFAULT;
	test_exerciseInfo.guidelineOffset = 0.0;
	test_exerciseInfo.distanceWeight = 1;
	test_exerciseInfo.altitudeWeight = 2;
	test_exerciseInfo.speedWeight = 3;
	test_exerciseInfo.targetSpeed = 4.0;
	test_exerciseInfo.accuracy = 5;
	test_exerciseInfo.pausingTime = 6;
	test_exerciseInfo.targetBearings = 7;
	test_exerciseInfo.showWaypoints = true;
	test_exerciseInfo.showGuideline = true;
	test_exerciseInfo.scoringMethod = ScoringMethod::MAE;

	configuration::FlightConfig test_flightConfig(
        test_operatorPosition,
		test_waypoints,
		test_markers,
		test_obstaclkes,
		test_exerciseInfo,
		test_connectionsInfo[0]
    );

	std::cout << "Operator position: " << test_flightConfig.getOperatorPosition().latitude<< " " << test_flightConfig.getOperatorPosition().longitude << " " << test_flightConfig.getOperatorPosition().altitude << std::endl;
	std::cout << "ExerciseInfo: " << int(test_flightConfig.getExerciseInfo().coordinatesSystem) << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
