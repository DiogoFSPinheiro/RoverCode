
	g++ -o detectar_obstaculos map_parsing.cpp ./src/_Map//Depth_First_Search.cpp  ./src/_Map/A-star/A-star.cpp `pkg-config --cflags --libs opencv4`
