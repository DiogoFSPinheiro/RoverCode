#include "Map.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/matx.hpp"
#include <ostream>



Map_::Map_(cv::Mat map) : Astart(map)
{
	_map = map;
	_t_point = 10;
}

Map_::Map_(Map_ &copy) : Astart(copy._map)
{
	this->_map = copy._map;
	_t_point = copy._t_point;
}

Map_::~Map_()
{
	std::cout << "end" << std::endl;
}


Map_ &Map_::operator=(Map_ &ret)
{
	this->_map = ret._map;
	_t_point = ret._t_point;
	return(*this);
}

cv::Mat Map_::get_map()
{
	return(this->_map);
}

void Map_::write_map_point(cv::Point2f point)
{
    // Converte para inteiros (arredondando para baixo)
    cv::Point p1(static_cast<int>(point.x), static_cast<int>(point.y));
    cv::Point p2(p1.x + 2, p1.y + 2);

    // Desenha o retângulo preenchido verde
    cv::rectangle(this->_map, p1, p2, cv::Scalar(0, 255, 0), cv::FILLED);
}



cv::Vec3b Map_::get_color(cv::Point2f p1)
{
    cv::Vec3b cor = _map.at<cv::Vec3b>(p1);
    return (cor);
}


bool Map_::set_new_point(cv::Point2f start , cv::Point2f end)
{
	cv::Point2f dir =  end - start;
	//std::cout << dir.x << " " << dir.y << std::endl;
	cv::Point2f new_dir;
	cv::Point2f new_point;
	cv::Vec3b color;
	float t = 0;
	float normal =  std::sqrt(dir.x * dir.x + dir.y * dir.y);	
	new_dir = dir * (  1.0f / normal);

	t = (dir.x * new_dir.x) + (dir.y * new_dir.y);

	for(int i = -1 ; i < (int)t;i++ )	
	{
		new_point = start + new_dir * i;
		color = this->get_color(new_point);
		if((int)color[1] == 0 &&  (int)color[0] == 0 && (int)color[2] == 255)
			return(false);

	}
	for(int i = -1 ; i < (int)t;i++ )	
	{
		new_point = start + new_dir * i;
		write_map_point(new_point);

	}
	
	return(true);
}

cv::Point2f Map_::set_new_point_t(cv::Point2f start , cv::Point2f end, int t)
{
	cv::Point2f dir =  end - start;
	//std::cout << dir.x << " " << dir.y << std::endl;
	cv::Point2f new_dir;
	cv::Point2f new_point;
	cv::Vec3b color;


	float normal =  std::sqrt(dir.x * dir.x + dir.y * dir.y);	
	new_dir = dir * (  1.0f / normal);
	//std::cout <<  t << std::endl;
	new_point = start + new_dir * t;
	return(new_point);
}


cv::Point2f 	Map_::get_dir(float angle, cv::Point2f points)
{
	cv::Point2f point;
	float rad;

	rad = angle * M_PI / 180;
	point.x = 1 * cos(rad);
	point.y = 1 * sin(rad);
	point.x =  points.x + point.x  ;
	point.y =  points.y + point.y  ;
	return(point);
}

float 	Map_::get_dir(cv::Point2f point)
{
	float rad;

	rad = atan2(point.x,point.y);
	return(rad * (180 / M_PI));
}


