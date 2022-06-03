RQ		= request

RS		= response

СС      = с++

SRC_S    =   main.cpp $(RQ)/request.cpp $(RQ)/request_func.cpp routines.cpp \
			$(RS)/response.cpp $(RS)/response_func.cpp \
			temp/Location.cpp

OBJ_S    =   ${SRC_S:.c=.o}

.c.o:
			cc ${FLAGS} -c $< -o ${<:.c=.o}

FLAGS   =   -Wall -Wextra -Werror -I headers -I response -I request -I temp

NAME	  =	 webserv

${NAME} : ${OBJ_S}
		c++ $(FLAGS) -o $(NAME) $(OBJ_S) -std=c++98

all:	${NAME}

clean:
			rm -f ${OBJ_S}

fclean:		clean
			rm -f $(NAME)

re:			fclean all

bonus: 		$(NAME)

.PHONY:		all clean fclean re bonus
