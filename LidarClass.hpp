#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <mutex>

const double PI = 3.14159265358979323846;
const int MAP_SIZE = 100;  // 100x100 células
const double SCALE = 0.1;  // metros por célula 

struct PolarPoint
{
    double angle;
    double distance; 
};

struct CartesianPoint
{
    int x;
    int y;
};

class LidarMap
{
	private:

		std::vector<std::vector<int>> map;
		std::mutex map_mutex;

		CartesianPoint polarToCartesian(const PolarPoint& point)
		{
			double angle_rad = point.angle * PI / 180.0;
			int x = static_cast<int>(std::round((point.distance * cos(angle_rad)) / SCALE + MAP_SIZE / 2));
			int y = static_cast<int>(std::round((point.distance * sin(angle_rad)) / SCALE + MAP_SIZE / 2));
			return { x, y };
		}

	public:

		LidarMap() : map(MAP_SIZE, std::vector<int>(MAP_SIZE, 0)) {}


		void addLidarPoint(const PolarPoint& point)
		{
			CartesianPoint cart = polarToCartesian(point);
			if (cart.x >= 0 && cart.x < MAP_SIZE && cart.y >= 0 && cart.y < MAP_SIZE)
			{
				std::lock_guard<std::mutex> lock(map_mutex);
				map[cart.y][cart.x] = 1;  // marca como obstáculo
			}
		}

		void printMap()
		{
			std::lock_guard<std::mutex> lock(map_mutex);
			for (const auto& row : map)
			{
				for (int cell : row)
					std::cout << (cell ? '#' : '.');
				std::cout << std::endl;
			}
		}
};
