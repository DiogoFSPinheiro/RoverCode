#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include "./src/_Map/Map.hpp"
#include "src/_Map/A-star/A-star.hpp"

#define IMG_T resultado

#define  JNL "janela"
cv::Mat ttr_(cv::Point start, cv::Point end, cv::Mat img)
{
	cv::Mat img_c;



	return img_c;	
}

   
void drawRectangle(int action, int x, int y, int flags, void *userdata)
{
	static cv::Point p1(0,0);
	static cv::Point p2(0,0);

	Map_ *map_ = (Map_ *) userdata;
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

int main() {
    // Carrega a imagem
    cv::Mat imagem = cv::imread("./maps/map.jpg");
    if (imagem.empty()) {
        std::cerr << "Erro ao carregar a imagem!" << std::endl;
        return -1;
    }
	
    
 
    Map_ *map_ = new Map_(imagem);
    
    map_->set_the_obstacles();
    cv::namedWindow(JNL, cv::WINDOW_NORMAL);
    cv::resizeWindow(JNL, 800, 600);

    // passa 0 no userdata (mesmo que não uses)
    cv::setMouseCallback(JNL, drawRectangle, map_);

    while (true) {
        cv::imshow(JNL, map_->get_map());
        if (cv::waitKey(30) == 27) break; // ESC sai
}
    
    return 0;
}

