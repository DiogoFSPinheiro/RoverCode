#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ostream>
#include "./src/_Map/Depth_First_Search.hpp"
#include "src/_Map/A-star/A-star.hpp"

#define IMG_T resultado


cv::Mat ttr_(cv::Point start, cv::Point end, cv::Mat img)
{
	cv::Mat img_c;



	return img_c;	
}

int main() {
    // Carrega a imagem
    cv::Mat imagem = cv::imread("./maps/map.jpg");
    if (imagem.empty()) {
        std::cerr << "Erro ao carregar a imagem!" << std::endl;
        return -1;
    }
	
    // Converte para escala de cinza
    cv::Mat cinza;
    cv::cvtColor(imagem, cinza, cv::COLOR_BGR2GRAY);

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
    cv::Mat resultado = imagem.clone();
    for (size_t i = 0; i < contornos.size(); ++i) {
        double area = cv::contourArea(contornos[i]);
        if (area > 0.2) { // define um limite mínimo de área para ser considerado obstáculo
            cv::drawContours(IMG_T, contornos, (int)i, cv::Scalar(0, 0, 255), 2); // vermelho = obstáculo
        }
    }

    // Mostra o resultado
    

const int SIZE = 5;  // Tamanho do mapa (5x5 neste exemplo)

    // Matriz definida manualmente
    int mapData[SIZE][SIZE] = {
        {3, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 2, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    // Criar imagem
    cv::Mat img(SIZE, SIZE, CV_8UC3);

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            switch (mapData[y][x]) {
                case 0: img.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0); break;        // Preto
                case 1: img.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255); break;  // Branco
                case 2: img.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0); break;      // Verde
                case 3: img.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0); break;      // Vermelho
                default: img.at<cv::Vec3b>(y, x) = cv::Vec3b(128, 128, 128); break; // Cinza
            }
        }
    }

    // Ampliar para ver melhor
  

    Astart *map_ = new Astart(resultado);

    
    cv::Point p1(90,104);
    cv::Point p2(403,44);

    
 //  map_->write_map_point(p1);
  // map_->write_map_point(p2);

   map_->start_al(p1, p2);
   map_->rute_to_map();
//   map_->escrever_caminho(p2);
   // map_->set_rute(p1,p2);
    //
    
  //  std::cout <<  "cor e "<<  (int)map_->get_color(p1)[1] << std::endl;

    cv::namedWindow("Obstáculos detectados", cv::WINDOW_NORMAL);
    cv::resizeWindow("Obstáculos detectados", 800, 600); // Define o tamanho da janela

    cv::imshow("Obstáculos detectados", map_->get_map());

    cv::waitKey(0);

    return 0;
}

