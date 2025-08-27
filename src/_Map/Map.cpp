#include "Map.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/matx.hpp"
#include <ostream>



Map_::Map_(cv::Mat map) : Astart(map)
{
	_map = map;
	_map_orig = map.clone();
	_t_point = 10;
}

Map_::Map_(Map_ &copy) : Astart(copy._map)
{
	this->_map = copy._map.clone();
	_map_orig = copy._map_orig.clone();
	_t_point = copy._t_point;
}

Map_::~Map_()
{
	std::cout << "end" << std::endl;
}


Map_ &Map_::operator=(Map_ &ret)
{
	this->_map = ret._map;
	this->_map_orig = ret._map_orig.clone();
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


void Map_::set_the_obstacles()
{

// Converte para escala de cinza
    cv::Mat cinza;
    cv::cvtColor(_map, cinza, cv::COLOR_BGR2GRAY);

    // Reduz ruído
    cv::Mat borrada;
    cv::GaussianBlur(cinza, borrada, cv::Size(1, 1), 0);

    // Detecção de bordas (Canny)
    cv::Mat bordas;
    cv::Canny(borrada, bordas, 50, 150);

    // Encontra contornos
    std::vector<std::vector<cv::Point>> contornos;
    cv::findContours(bordas, contornos, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Desenha os contornos detectados
    cv::Mat resultado = _map.clone();
    for (size_t i = 0; i < contornos.size(); ++i) {
        double area = cv::contourArea(contornos[i]);
        if (area > 0.2) { // define um limite mínimo de área para ser considerado obstáculo
            cv::drawContours(_map, contornos, (int)i, cv::Scalar(0, 0, 255), 2); // vermelho = obstáculo
        }
    }
}

void Map_::mat_clear()
{
	this->_map = this->_map_orig.clone();
	new_map(_map);
}
