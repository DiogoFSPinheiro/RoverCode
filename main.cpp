



#include "LidarClass.hpp"


int main()
{
	LidarMap map;

	// Simulação de dados LIDAR
	std::vector<PolarPoint> scan = { {0, 2.0}, {45, 1.5}, {90, 3.0}, {135, 1.0}, {180, 2.5}, {225, 1.0}, {270, 2.0}, {90, 1.5}, {42, 2.9}, };

	for (const auto& p : scan)
		map.addLidarPoint(p);


	map.printMap();
	return 0;
}
