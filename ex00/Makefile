#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lucorrei <lucorrei@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/31 13:57:17 by lucorrei          #+#    #+#              #
#    Updated: 2025/11/19 13:24:11 by lucorrei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = btc

HEADER = BitCoinExchange.hpp

CPPFILES =  BitCoinExchange.cpp main.cpp

OBJS := $(CPPFILES:.cpp=.o)

CPP = c++

CPPFLAGS =  -std=c++98 -Wall -Wextra -Werror -ggdb3

all: ${NAME}

${NAME}:  ${OBJS}
	${CPP} ${OBJS} ${CPPFLAGS} -o ${NAME}

%.o: %.cpp ${HEADER}
	${CPP} -c ${CPPFLAGS} $< -o $@

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: fclean clean all re test
