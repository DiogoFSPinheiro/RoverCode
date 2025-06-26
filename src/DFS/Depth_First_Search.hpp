#ifndef Depth_First_Search

#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>

class DFS 
{
	private:
		cv::Mat _map;
	
	public:
	  DFS(cv::Mat( map));
	  DFS(DFS & copy);
	  DFS &operator=(DFS &ret);
	  ~DFS();
	  cv::Mat get_map();
	  void write_map_point(cv::Point);

};


#endif
