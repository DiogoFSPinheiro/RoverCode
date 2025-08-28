#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include "../../_Map/Map.hpp"
#include "../../_Map/A-star/A-star.hpp"
#include "include/all.hpp"


void event_mause_Lbutton( int action, int x, int y ,Map_ *map_)
{

	static cv::Point p1(0,0);
	static cv::Point p2(0,0);
	if (action == cv::EVENT_LBUTTONDOWN)
	{
		if(p1.x != 0 && p1.y != 0)
			p2 = cv::Point(x,y);
		else
		{
		 	p1 =  cv::Point(x,y);
			return;
		}
		if(p1.x != 0 && p1.y != 0)
		{
			map_->mat_clear();

    			map_->set_the_obstacles();
   			map_->start_al(p1, p2);
   			map_->rute_to_map(map_->get_map());
			
        		cv::imshow(JNL, map_->get_map());
		 	p1 =  cv::Point(0,0);
		 	p2 =  cv::Point(0,0);

		}

		std::cout << "Clique em (" << x << ", " << y << ")\n";
	}

}


