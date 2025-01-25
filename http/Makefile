# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/25 20:30:46 by cteoh             #+#    #+#              #
#    Updated: 2025/01/25 22:06:05 by cteoh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= libhttp.a

CXX			:= c++
CXXFLAGS	= -Wall -Werror -Wextra -std=c++98 $(include_dir)

AR			:= ar -rcs

src_dir 	:= src src/coreABNF src/URI
dirs		:= $(src_dir)

full_srcs	:= $(foreach dir, $(dirs), $(wildcard $(dir)/*.cpp))
srcs		:= $(notdir $(full_srcs))
vpath %.cpp $(shell find src/ -type d | tr '\n' : | awk '{print substr($$1, 0, length($$1)-1)}')

include_dir	:= $(addprefix -I, $(shell find include/ -type d))

obj_dir		:= obj
objs		:= $(srcs:%.cpp=$(obj_dir)/%.o)

all: $(NAME) test

test:
	$(CXX) $(CXXFLAGS) tests.cpp $(NAME)
	./a.out

$(NAME): $(objs)
	$(AR) $(NAME) $(objs)

$(obj_dir):
	mkdir -p $(obj_dir)

$(obj_dir)/%.o: %.cpp | obj
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(includes) $< -c -o $@

clean:
	$(RM) -r $(obj_dir)

fclean: clean
	$(RM) $(NAME) a.out

re: fclean all

fsan: CXXFLAGS += -fsanitize=address,undefined -g3
fsan: all

.PHONY: all clean fclean re fsan
