#include "A-star.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/matx.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <iterator>
#include <list>
#include <algorithm> // para std::sort
#include <ostream>

Astart::Astart(cv::Mat map) : Map_(map)
{
}

Astart::Astart(Astart &copy) : Map_(copy)
{
}

Astart::~Astart()
{
	std::cout << "end" << std::endl;
}


Astart &Astart::operator=(Astart &ret)
{
	return(*this);
}


auto  Astart::chek_points(cv::Point target)
{
	auto it = std::find_if(_open.begin(), _open.end(), [&](const t_node& node)
	{
			return node.point == target;  // compara x e y
	});
	if(it != _open.end())
		return (it);
	it = std::find_if(_close.begin(), _close.end(), [&](const t_node& node)
	{
			return node.point == target;  // compara x e y
	});
	
	return (it);

}

bool  Astart::chek_valid_point(t_node node)
{

	if(node.point.x > _map.rows || node.point.y > _map.cols || node.point.x < 0 || node.point.y < 0 )
		return (false);
	cv::Vec3i color= this->get_color(node.point);
	if(color[0] == 0  && color[1] == 0 && color[2] == 255)
		return (false);
	auto node_che = chek_points(node.point);
	if(node_che == _open.end() ||  node_che == _close.end() )
		return (true);	
	return (false);
}


double Astart::heuristic(cv::Point a, cv::Point b) {
    // Distância Manhattan
    return abs(a.x - b.x) + abs(a.y - b.y);
}


t_node Astart::create_node(cv::Point point, cv::Point prev, cv::Point goal)
{
    t_node node;
    
    // Busca nó anterior
    auto node_prev = chek_points(prev);  
    if (prev == point)
        // Se não existe nó anterior (ex: início), g = 0
        node.g = 0;
    else
        node.g = node_prev->g + 1;  // custo acumulado
    
    node.point = point;
    node.previos_point = prev;

    // Calcular heurística h
    node.h = heuristic(point, goal);

    // Calcular f = g + h
    node.f = node.g + node.h;

    return node;
}


void Astart::node_espand(s_node node,cv::Point end)
{
	t_node node_[4];
	cv::Point p[4];
	
	p[0] = node.point;
	p[1] = node.point;
	p[2] = node.point;
	p[3] = node.point;
	p[0].x++;
	p[1].x--;
	p[2].y++;
	p[3].y--;
        node_[0] = create_node(p[0], node.point, end);
        node_[2] = create_node(p[1], node.point, end);
        node_[2] = create_node(p[2], node.point, end);
        node_[3] = create_node(p[3], node.point, end);
		
	_close.splice(_close.end(), _open, chek_points(node.point));
	std::cout << "node " << node.point << std::endl;
	for(int i = 0; i < 4;i++)
	{
		if(node_[i].point == end)
		{
			std::cout << "ok _end" << std::endl;
			_close.splice(_close.end(), _open);	
			_close.emplace_front(node_[i]);
			return;
		}
		if(chek_valid_point(node_[i]) == true)
		{
			_open.emplace_front(node_[i]);
		}
	}

}


bool Astart::start_al(cv::Point start, cv::Point end)
{
	t_node node;
	t_node copy;
	cv::Point point;
	cv::Point temp;

	_open.clear();
	_close.clear();
	_rute.clear();
	node  = create_node(start, start, end);
	copy = node;
	_open.emplace_front(node);	
	while (!_open.empty())
	{
		_open.sort([](const t_node& a,const  t_node& b) {
        	return a.f < b.f;  // Ordena por menor f
   		});
		node_espand(*_open.begin(),end);	
	}
	point = end;

	while ( chek_points(point)->point != start && chek_points(point) != _close.end())
	{

		std::cout << point << std::endl;
		temp = point;
		point = chek_points(point)->previos_point;
		_rute.splice(_rute.end(), _rute, chek_points(temp));			
		temp = point;
		
	}

	_rute.splice(_rute.end(), _rute, chek_points(start));			
	return (true);
}


void Astart::rute_to_map()
{
	for(auto& node :_rute)		
	{
	//write_map_point(node.point);
		_map.at<cv::Vec3b>(node.point.y, node.point.x) = cv::Vec3b(0, 230, 0);
	}
}
