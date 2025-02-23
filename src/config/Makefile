# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/12 22:06:13 by kecheong          #+#    #+#              #
#    Updated: 2025/02/21 18:17:45 by kecheong         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX := g++
SERVER_INC := ../../include
INC := -I $(SERVER_INC) -I $(SERVER_INC)/utils
CXXFLAGS := -Wall -Werror -Wextra -std=c++98 -fsanitize=address,undefined -g3 \
			$(INC)

LDFLAGS := -fuse-ld=mold

LIBSERV := ../../libserv.a

SRCS := main.cpp Token.cpp Lexer.cpp Parser.cpp Directive.cpp \
		Validator.cpp Configurator.cpp ConfigErrors.cpp Configuration.cpp \
		Context.cpp

OBJS := $(SRCS:.cpp=.o)

all: parser

parser: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o parser $(LIBSERV) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) parser

re: fclean all
