RQ		= request

RS		= response

CG		= cgi

СС      = с++

SRC_S    =   main.cpp Iosck.cpp $(RQ)/request.cpp $(RQ)/request_func.cpp routines.cpp \
			$(RS)/response.cpp $(RS)/response_func.cpp $(CG)/cgi.cpp \
			$(CG)/cgi_routines.cpp temp/Location.cpp

OBJ_S    =   ${SRC_S:.cpp=.o}

.cpp.o:
			c++ ${FLAGS} -c $< -o ${<:.cpp=.o}

FLAGS   =   -Wall -Wextra -Werror -I headers -I response -I request -I cgi -I temp -I.

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
