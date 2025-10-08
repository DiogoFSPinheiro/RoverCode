
#include "includes/LidarClass.hpp"


int main()
{
	LidarMap map;

	// Simulação de dados LIDAR - Mars terrain with rocks, boulders, and craters
	std::vector<PolarPoint> scan = { 
		// Large boulder cluster to the north
		{10, 3.2}, {15, 3.1}, {20, 3.0}, {25, 3.1}, {30, 3.3},
		
		// Rocky outcrop to the northeast  
		{45, 2.8}, {50, 2.5}, {55, 2.7}, {60, 2.9},
		
		// Scattered rocks to the east
		{85, 4.1}, {90, 3.8}, {95, 4.0},
		
		// Crater rim to the southeast
		{120, 2.2}, {125, 2.0}, {130, 1.8}, {135, 2.1}, {140, 2.4},
		
		// Large rock formation to the south
		{170, 3.5}, {175, 3.7}, {180, 3.9}, {185, 3.6}, {190, 3.4},
		
		// Medium rocks to the southwest
		{210, 2.6}, {220, 2.8}, {230, 2.5},
		
		// Boulder field to the west
		{260, 3.0}, {270, 2.9}, {280, 3.1}, {285, 3.2},
		
		// Small rocks to the northwest
		{310, 1.9}, {320, 2.1}, {330, 1.8}, {340, 2.0}
	};

	std::cout << "Adding LIDAR obstacles to the map..." << std::endl;
	for (const auto& p : scan)
		map.addLidarPoint(p);


	std::cout << "\nInflated map (accounting for 1m x 1m car size):" << std::endl;
	map.printInflatedMap();

	std::cout << "\n" << std::string(50, '=') << std::endl;
	std::cout << "PATHFINDING DEMONSTRATIONS" << std::endl;
	std::cout << std::string(50, '=') << std::endl;

	// Test 1: Simple path in open space
	std::cout << "\nTest 1: Path in open space" << std::endl;
	CartesianPoint start1 = {10, 10};
	CartesianPoint goal1 = {20, 20};
	std::cout << "Finding path from (" << start1.x << ", " << start1.y << ") to (" << goal1.x << ", " << goal1.y << ")" << std::endl;
	std::vector<CartesianPoint> path1 = map.findPath(start1, goal1);
	map.printMapWithPath(path1);

	// Test 2: Corner to corner path - Bottom-left to Top-right
	std::cout << "\nTest 2: Corner to corner path (Bottom-left to Top-right)" << std::endl;
	CartesianPoint start2 = {5, 95};   // Bottom-left corner
	CartesianPoint goal2 = {95, 5};    // Top-right corner
	std::cout << "Finding path from (" << start2.x << ", " << start2.y << ") to (" << goal2.x << ", " << goal2.y << ")" << std::endl;
	std::vector<CartesianPoint> path2 = map.findPath(start2, goal2);
	map.printMapWithPath(path2);

	// Test 3: Corner to corner path - Top-left to Bottom-right
	std::cout << "\nTest 3: Corner to corner path (Top-left to Bottom-right)" << std::endl;
	CartesianPoint start3 = {5, 5};    // Top-left corner
	CartesianPoint goal3 = {95, 95};   // Bottom-right corner
	std::cout << "Finding path from (" << start3.x << ", " << start3.y << ") to (" << goal3.x << ", " << goal3.y << ")" << std::endl;
	std::vector<CartesianPoint> path3 = map.findPath(start3, goal3);
	map.printMapWithPath(path3);


	std::cout << "\n" << std::string(50, '=') << std::endl;
	std::cout << "PATHFINDING TESTS COMPLETED" << std::endl;
	std::cout << std::string(50, '=') << std::endl;

	return 0;
}
