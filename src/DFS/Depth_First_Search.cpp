#include "Depth_First_Search.hpp"
#include "opencv2/core/mat.hpp"
#include <ostream>


DFS::DFS(cv::Mat map)
{
	_map = map;
}

DFS::DFS(DFS &copy)
{
	this->_map = copy._map;
}

DFS::~DFS()
{
	std::cout << "end" << std::endl;
}


DFS &DFS::operator=(DFS &ret)
{
	this->_map = ret._map;
	return(*this);
}

cv::Mat DFS::get_map()
{
	return(this->_map);
}

void DFS::write_map_point(cv::Point point)
{

    cv::Point p2(point.x+2,point.y+2);
    cv::rectangle(this->_map,point,p2, cv::Scalar(0,255,0),cv::FILLED);
}
