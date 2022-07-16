RNQ		= req_n_resp

RQ		= request

RS		= response

CG		= cgi

PR		= website/parser_copy

СС      = с++

SRC_S    =  $(RNQ)/main.cpp $(RNQ)/$(RQ)/request.cpp $(RNQ)/$(RQ)/request_func.cpp \
			$(RNQ)/routines.cpp $(RNQ)/$(RS)/response.cpp \
			$(RNQ)/$(RS)/response_func.cpp $(RNQ)/$(CG)/cgi.cpp \
			$(RNQ)/$(CG)/cgi_routines.cpp \
			$(PR)/Location.cpp $(PR)/ParserConfig.cpp $(PR)/Server.cpp


OBJ_S    =   ${SRC_S:.cpp=.o}

.cpp.o:
			c++ ${FLAGS} -c $< -o ${<:.cpp=.o}

FLAGS   =   -Wall -Wextra -Werror -I $(RNQ)/headers -I $(RNQ)/$(RS) -I $(RNQ)/$(RQ) \
			-I $(RNQ)/$(CG) -I $(PR)

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
