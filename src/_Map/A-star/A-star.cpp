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

Astart::Astart(cv::Mat map) 
{
	this->_map = map;
}

Astart::Astart(Astart &copy) 
{

	this->_map = copy._map.clone();
}

Astart::~Astart()
{
	std::cout << "end" << std::endl;
}


Astart &Astart::operator=(Astart &ret)
{
	return(*this);
}


cv::Vec3b Astart::get_color(cv::Point2f p1)
{
    cv::Vec3b cor = _map.at<cv::Vec3b>(p1);
    return (cor);
}


auto  Astart::check_point(cv::Point target)
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

bool  Astart::is_valid_point(t_node node)
{

	if(node.point.x > _map.cols || node.point.y > _map.rows || node.point.x < 0 || node.point.y < 0 )
		return (false);
	cv::Vec3i color= this->get_color(node.point);
	if(color[0] == 0  && color[1] == 0 && color[2] == 255)
		return (false);
	auto node_che = check_point(node.point);
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
    auto node_prev = check_point(prev);  
    if ( node_prev == _open.end() || node_prev == _close.end()|| prev == point)
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


void Astart::expand_node(s_node node,cv::Point end)
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
        node_[1] = create_node(p[1], node.point, end);
        node_[2] = create_node(p[2], node.point, end);
        node_[3] = create_node(p[3], node.point, end);
		
	_close.splice(_close.end(), _open, check_point(node.point));
	//std::cout << "node " << node.point << std::endl;
	for(int i = 0; i < 4;i++)
	{
		if(node_[i].point == end)
		{
			_close.splice(_close.end(), _open);	
			_close.emplace_front(node_[i]);
			return;
		}
		if(is_valid_point(node_[i]) == true)
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
	_route.clear();

	node  = create_node(start, start, end);
	copy = node;
	cv::Vec3i color= this->get_color(end);
	if(color[0] == 0  && color[1] == 0 && color[2] == 255)
		return false ;	
	color= this->get_color(start);
		if(color[0] == 0  && color[1] == 0 && color[2] == 255)
		return false ;	
	_open.emplace_front(node);	
	while (!_open.empty())
	{
		if(_open.size() > 2)
		{
			_open.sort([](const t_node& a,const  t_node& b) {
        		return a.f < b.f;  // Ordena por menor f
   			});
		}
		expand_node(*_open.begin(),end);	
	}
	point = end;
	
	std::cout << point << std::endl;
	while ( check_point(point)->point != start && check_point(point) != _close.end())
	{

		temp = point;
		point = check_point(point)->previos_point;
		_route.splice(_route.end(), _route, check_point(temp));
		temp = point;
		
	}
	_route.splice(_route.end(), _route, check_point(start));			
	return (true);
}


void Astart::rute_to_map()
{
	for(auto& node :_route)		
	{
		_map.at<cv::Vec3b>(node.point.y, node.point.x) = cv::Vec3b(0, 230, 0);
	}
}
