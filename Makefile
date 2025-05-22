NAME = map_obj_viw

CC = g++
FLAG =  `pkg-config --cflags --libs opencv4`
SRC = map_parsing.cpp
SRCS = $(SRC:.cpp=.o)

all: $(NAME)

	g++ -o detectar_obstaculos map_parsing.cpp `pkg-config --cflags --libs opencv4`

$(NAME): $(SRCS)


re:
	make fclean
	make all

clean:
	$(SRCS)

fclean:
	rm $(SRCS) $(NAME)
