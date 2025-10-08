#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <mutex>
#include <queue>
#include <algorithm>
#include <climits>

const double PI = 3.14159265358979323846;
const int MAP_SIZE = 100;  // 100x100 células
const double SCALE = 0.1;  // metros por célula, 10 metros de mapa
const double CAR_SIZE = 1.0;  // Car size in meters (1m x 1m)
const int CAR_PADDING = static_cast<int>(std::ceil(CAR_SIZE / (2 * SCALE)));  // Padding in grid cells (5 cells)

// ANSI Color codes for terminal output
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string CYAN = "\033[36m";

struct PolarPoint
{
	double angle;
	double distance; 
};

struct CartesianPoint
{
	int x;
	int y;

	bool operator==(const CartesianPoint& other) const {
		return x == other.x && y == other.y;
	}
};

struct PathNode
{
	CartesianPoint point;
	double gCost;  // Distance from start
	double hCost;  // Heuristic distance to goal
	double fCost;  // Total cost (g + h)
	CartesianPoint parent;
	bool hasParent;

	PathNode() : gCost(0), hCost(0), fCost(0), hasParent(false) {}
	
	PathNode(CartesianPoint p) : point(p), gCost(0), hCost(0), fCost(0), hasParent(false) {}

	bool operator>(const PathNode& other) const {
		return fCost > other.fCost;
	}
};

class LidarMap
{
	private:

		std::vector<std::vector<int>> map;
		std::vector<std::vector<int>> inflatedMap;  // Map with inflated obstacles for pathfinding
		mutable std::mutex map_mutex;

		CartesianPoint polarToCartesian(const PolarPoint& point)
		{
			double angle_rad = point.angle * PI / 180.0;
			int x = static_cast<int>(std::round((point.distance * cos(angle_rad)) / SCALE + MAP_SIZE / 2));
			int y = static_cast<int>(std::round((point.distance * sin(angle_rad)) / SCALE + MAP_SIZE / 2));
			return { x, y };
		}

		// Inflate obstacles to account for car size (minimal padding for smooth curves)
		void inflateObstacles()
		{
			inflatedMap = map;  // Start with original map
			
			// For each obstacle in the original map, add minimal padding
			for (int y = 0; y < MAP_SIZE; y++)
			{
				for (int x = 0; x < MAP_SIZE; x++)
				{
					if (map[y][x] == 1)  // If there's an obstacle
					{
						// Add minimal padding around this obstacle (just adjacent cells)
						for (int dy = -CAR_PADDING; dy <= CAR_PADDING; dy++)
						{
							for (int dx = -CAR_PADDING; dx <= CAR_PADDING; dx++)
							{
								int newX = x + dx;
								int newY = y + dy;
								
								// Check bounds and mark as obstacle
								if (newX >= 0 && newX < MAP_SIZE && newY >= 0 && newY < MAP_SIZE)
								{
									inflatedMap[newY][newX] = 1;
								}
							}
						}
					}
				}
			}
		}

		// Helper function to calculate Euclidean distance between two points
		double calculateDistance(const CartesianPoint& a, const CartesianPoint& b)
		{
			return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
		}

		// Check if a point is valid (within bounds and not an obstacle)
		bool isValidPoint(const CartesianPoint& point)
		{
			return point.x >= 0 && point.x < MAP_SIZE && 
				   point.y >= 0 && point.y < MAP_SIZE && 
				   inflatedMap[point.y][point.x] == 0;  // Use inflated map for pathfinding
		}

		// Get valid neighboring points (8-directional movement)
		std::vector<CartesianPoint> getNeighbors(const CartesianPoint& point)
		{
			std::vector<CartesianPoint> neighbors;
			int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
			int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

			for (int i = 0; i < 8; i++)
			{
				CartesianPoint neighbor = {point.x + dx[i], point.y + dy[i]};
				if (isValidPoint(neighbor))
				{
					neighbors.push_back(neighbor);
				}
			}
			return neighbors;
		}

	public:

		LidarMap() : map(MAP_SIZE, std::vector<int>(MAP_SIZE, 0)), 
					 inflatedMap(MAP_SIZE, std::vector<int>(MAP_SIZE, 0)) {}


		void addLidarPoint(const PolarPoint& point)
		{
			CartesianPoint cart = polarToCartesian(point);
			if (cart.x >= 0 && cart.x < MAP_SIZE && cart.y >= 0 && cart.y < MAP_SIZE)
			{
				std::lock_guard<std::mutex> lock(map_mutex);
				map[cart.y][cart.x] = 1;
				inflateObstacles();  // Regenerate inflated map after adding obstacle
			}
		}

		void printMap()
		{
			std::lock_guard<std::mutex> lock(map_mutex);
			std::cout << "Original map (# = obstacles, . = free space):" << std::endl;
			for (const auto& row : map)
			{
				for (int cell : row)
				{
					if (cell == 1)
						std::cout << RED << '#' << RESET;  // Red obstacles
					else
						std::cout << '.';  // Normal free space
				}
				std::cout << std::endl;
			}
		}

		void printInflatedMap()
		{
			std::lock_guard<std::mutex> lock(map_mutex);
			std::cout << "Inflated map (# = obstacles + car padding, . = free space):" << std::endl;
			for (const auto& row : inflatedMap)
			{
				for (int cell : row)
				{
					if (cell == 1)
						std::cout << RED << '#' << RESET;  // Red obstacles
					else
						std::cout << '.';  // Normal free space
				}
				std::cout << std::endl;
			}
		}

		// A* pathfinding algorithm to find path from point A to point B
		std::vector<CartesianPoint> findPath(const CartesianPoint& start, const CartesianPoint& goal)
		{
			std::lock_guard<std::mutex> lock(map_mutex);
			
			std::vector<CartesianPoint> path;
			
			// Check if start and goal are valid
			if (!isValidPoint(start) || !isValidPoint(goal))
			{
				std::cout << "Start or goal point is invalid!" << std::endl;
				return path;
			}

			// If start equals goal, return path with just the start point
			if (start == goal)
			{
				path.push_back(start);
				return path;
			}

			// Priority queue for open set (nodes to be evaluated)
			std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>> openSet;
			
			// Track visited nodes
			std::vector<std::vector<bool>> closedSet(MAP_SIZE, std::vector<bool>(MAP_SIZE, false));
			std::vector<std::vector<PathNode>> nodeMap(MAP_SIZE, std::vector<PathNode>(MAP_SIZE));

			// Initialize start node
			PathNode startNode(start);
			startNode.gCost = 0;
			startNode.hCost = calculateDistance(start, goal);
			startNode.fCost = startNode.gCost + startNode.hCost;
			
			openSet.push(startNode);
			nodeMap[start.y][start.x] = startNode;

			while (!openSet.empty())
			{
				// Get node with lowest f cost
				PathNode current = openSet.top();
				openSet.pop();

				// Mark as visited
				closedSet[current.point.y][current.point.x] = true;

				// Check if we reached the goal
				if (current.point == goal)
				{
					// Reconstruct path
					CartesianPoint pathPoint = goal;
					while (nodeMap[pathPoint.y][pathPoint.x].hasParent)
					{
						path.push_back(pathPoint);
						pathPoint = nodeMap[pathPoint.y][pathPoint.x].parent;
					}
					path.push_back(start);
					std::reverse(path.begin(), path.end());
					return path;
				}

				// Check all neighbors
				std::vector<CartesianPoint> neighbors = getNeighbors(current.point);
				for (const CartesianPoint& neighbor : neighbors)
				{
					// Skip if already visited
					if (closedSet[neighbor.y][neighbor.x])
						continue;

					// Calculate movement cost (diagonal movement costs more)
					double movementCost = (abs(neighbor.x - current.point.x) + abs(neighbor.y - current.point.y) == 2) ? 1.414 : 1.0;
					double tentativeGCost = current.gCost + movementCost;

					// Check if this path to neighbor is better
					PathNode& neighborNode = nodeMap[neighbor.y][neighbor.x];
					bool inOpenSet = neighborNode.fCost > 0 || neighbor == start;

					if (!inOpenSet || tentativeGCost < neighborNode.gCost)
					{
						neighborNode.point = neighbor;
						neighborNode.gCost = tentativeGCost;
						neighborNode.hCost = calculateDistance(neighbor, goal);
						neighborNode.fCost = neighborNode.gCost + neighborNode.hCost;
						neighborNode.parent = current.point;
						neighborNode.hasParent = true;

						if (!inOpenSet)
						{
							openSet.push(neighborNode);
						}
					}
				}
			}

			// No path found
			std::cout << "No path found from (" << start.x << ", " << start.y << ") to (" << goal.x << ", " << goal.y << ")" << std::endl;
			return path;
		}

		// Print map with path visualized
		void printMapWithPath(const std::vector<CartesianPoint>& path)
		{
			std::lock_guard<std::mutex> lock(map_mutex);
			
			// Create a copy of the map for visualization
			std::vector<std::vector<char>> visualMap(MAP_SIZE, std::vector<char>(MAP_SIZE, '.'));
			
			// Mark only original obstacles (skip inflated padding for cleaner display)
			for (int y = 0; y < MAP_SIZE; y++)
			{
				for (int x = 0; x < MAP_SIZE; x++)
				{
					if (map[y][x] == 1)
						visualMap[y][x] = '#';  // Original obstacles
				}
			}
			
			// Mark path
			for (size_t i = 0; i < path.size(); i++)
			{
				int x = path[i].x;
				int y = path[i].y;
				if (x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE)
				{
					if (i == 0)
						visualMap[y][x] = 'S';  // Start
					else if (i == path.size() - 1)
						visualMap[y][x] = 'G';  // Goal
					else
						visualMap[y][x] = '*';  // Path
				}
			}
			
			// Print the map
			std::cout << "\nMap with path (S=Start, G=Goal, *=Path, #=Obstacles, .=Free):" << std::endl;
			std::cout << "Car size: " << CAR_SIZE << "m x " << CAR_SIZE << "m (safety padding applied invisibly)" << std::endl;
			for (const auto& row : visualMap)
			{
				for (char cell : row)
				{
					switch(cell)
					{
						case '#':  // Obstacles in red
							std::cout << RED << cell << RESET;
							break;
						case '*':  // Path in green
							std::cout << GREEN << cell << RESET;
							break;
						case 'S':  // Start in cyan
							std::cout << CYAN << cell << RESET;
							break;
						case 'G':  // Goal in yellow
							std::cout << YELLOW << cell << RESET;
							break;
						default:   // Free space (dots) in normal color
							std::cout << cell;
							break;
					}
				}
				std::cout << std::endl;
			}
		}
};
