

#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/imgproc.hpp"
#include <list>
#include <opencv2/opencv.hpp>


#ifndef A_start
#define A_start

#include "../Depth_First_Search.hpp"


typedef struct s_node
{
	cv::Point point;
	int g; // cuto de me mover ate aqui
	int h; // e uma estimativa da distacia
	int f; // e g+h;
	cv::Point previos_point;
} t_node;


class Astart : public Map_
{
	private:
		std::list<t_node> _open;
		std::list<t_node> _close;
		std::list<t_node> _rute;
		auto  chek_points(cv::Point);
	  	int Distace(cv::Point point,cv::Point end);
		bool  chek_valid_point(t_node node);
		t_node create_node(cv::Point point ,cv::Point prev,cv::Point end);
		double heuristic(cv::Point a, cv::Point b);
		void node_espand(s_node node,cv::Point end);		

	public:
	  Astart(cv::Mat( map));
	  Astart(Astart & copy);
	  Astart &operator=(Astart &ret);
	  ~Astart();
	  bool start_al(cv::Point start,cv::Point end);
	  void rute_to_map();
	  
};


#endif
