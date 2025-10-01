NAME = map_obj_viw

CC = g++
FLAG = `pkg-config --cflags --libs opencv4`
SRC = map_parsing.cpp ./src/_Map/Map.cpp  ./src/_Map/A-star/A-star.cpp

SRCS = $(SRC:.cpp=.o)

%.o:%.cpp
	$(CC) $(FLAG) -c $< -o $@


all: $(NAME)

	g++ -o $(NAME) $(SRCS) `pkg-config --cflags --libs opencv4`

$(NAME): $(SRCS)

docker:
	sudo xhost +local:docker
	sudo docker-compose up --build

re:fclean
	make all

clean:
	rm -fr $(SRCS)

fclean:clean
	rm -fr $(SRCS) $(NAME)

s:re
	./$(NAME)
	
