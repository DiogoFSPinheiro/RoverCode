



#include "LidarClass.hpp"


int main()
{
	LidarMap map;

	// Simulação de dados LIDAR
	std::vector<PolarPoint> scan = { {0, 2.0}, {45, 1.5}, {90, 3.0}, {135, 1.0}, {180, 2.5}, {225, 1.0}, {270, 2.0}, {90, 1.5}, {42, 2.9}, };

	std::cout << "Adding LIDAR obstacles to the map..." << std::endl;
	for (const auto& p : scan)
		map.addLidarPoint(p);

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

	// Test 2: Path around obstacles
	std::cout << "\nTest 2: Path around obstacles" << std::endl;
	CartesianPoint start2 = {30, 30};
	CartesianPoint goal2 = {70, 70};
	std::cout << "Finding path from (" << start2.x << ", " << start2.y << ") to (" << goal2.x << ", " << goal2.y << ")" << std::endl;
	std::vector<CartesianPoint> path2 = map.findPath(start2, goal2);
	map.printMapWithPath(path2);

	// Test 3: Short distance path
	std::cout << "\nTest 3: Short distance path" << std::endl;
	CartesianPoint start3 = {5, 5};
	CartesianPoint goal3 = {8, 8};
	std::cout << "Finding path from (" << start3.x << ", " << start3.y << ") to (" << goal3.x << ", " << goal3.y << ")" << std::endl;
	std::vector<CartesianPoint> path3 = map.findPath(start3, goal3);
	map.printMapWithPath(path3);


	std::cout << "\n" << std::string(50, '=') << std::endl;
	std::cout << "PATHFINDING TESTS COMPLETED" << std::endl;
	std::cout << std::string(50, '=') << std::endl;

	return 0;
}
