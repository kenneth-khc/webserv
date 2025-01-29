# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/16 16:45:20 by kecheong          #+#    #+#              #
#    Updated: 2025/01/29 22:41:16 by kecheong         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := debug_server

NAME := webserv

CXX := g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 $(includes)

src_dir := src
dirs := $(src_dir) \
		$(src_dir)/debug \
		$(src_dir)/methods
		# http			 \
		# http/src		 \
		# http/src/message \
		# http/src/coreABNF \
		# http/src/URI

srcs := $(foreach dir, $(dirs), $(wildcard $(dir)/*.cpp))
# For my own testing purposes
srcs += testServer.cpp
include_dir := include/ 

includes := -I include/ \
			-I http/	\
			-I http/include/ \
			-I http/include/message \
			-I http/include/coreABNF \
			-I http/include/URI


obj_dir := obj
objs := $(srcs:$(src_dir)/%.cpp=$(obj_dir)/%.o)

all: $(NAME)

$(NAME): $(objs) http/libhttp.a
	$(CXX) $(CXXFLAGS) $(objs) http/libhttp.a -o $(NAME)

http/libhttp.a:
	make -C http/

$(obj_dir):
	mkdir -p $(obj_dir)

vpath %.cpp http/ http/src http/src/message
$(obj_dir)/%.o: $(src_dir)/%.cpp | obj
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(includes) $< -c -o $@

clean:
	$(RM) -r $(obj_dir)

fclean: clean
	$(RM) $(NAME)

re: fclean all

optimized: CXXFLAGS += -O3
optimized: all

debug_server: CXXFLAGS += -g3
debug_server: all

fsan: CXXFLAGS += -fsanitize=address,undefined -g3
fsan: all

.PHONY: all clean fclean re optimized debug fsan
