#ifndef Map_HPP
#define Map_HPP

#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>

#include "A-star/A-star.hpp"

class Map_  : public Astart
{
	protected:
		cv::Mat _map;
		int _t_point;
	
	public:
	  Map_(cv::Mat( map));
	  Map_(Map_ & copy);
	  Map_ &operator=(Map_ &ret);
	  ~Map_();
	  cv::Mat 	get_map();
	  cv::Vec3b 	get_color(cv::Point2f);
	  void 		write_map_point(cv::Point2f);
	  cv::Point2f 	get_dir(float angle,  cv::Point2f points);
	  float 	get_dir(cv::Point2f point);


	  bool 		set_new_point(cv::Point2f start , cv::Point2f end);
	  cv::Point2f 	set_new_point_t(cv::Point2f start , cv::Point2f dir, int t); 
	  bool 		set_rute( cv::Point2f start , cv::Point2f end);


};


#endif
