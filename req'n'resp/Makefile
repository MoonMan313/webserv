SRC_S    =   main.cpp request.cpp request_func.cpp routines.cpp

OBJ_S    =   ${SRC_S:.cpp=.o}

FLAGS   =   -Wall -Wextra -Werror

NAME	  =	 webserv

${NAME} : ${OBJ_S}
		 c++  $(FLAGS) -o $(NAME) $(OBJ_S) -std=c++98

all:	${NAME}

clean:
			rm -f ${OBJ_S}

fclean:		clean
			rm -f $(NAME)

re:			fclean all

bonus: 		$(NAME)

.PHONY:		all clean fclean re bonus
