# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/16 16:45:20 by kecheong          #+#    #+#              #
#    Updated: 2025/02/05 05:07:08 by cteoh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := debug_server

NAME := webserv

CXX := g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 $(includes) -g3 -fsanitize=address,undefined

src_dir := src
dirs := $(src_dir) \
		$(src_dir)/debug \
		$(src_dir)/methods \
		$(src_dir)/server
		# http			 \
		# http/src		 \
		# http/src/headers \
		# http/src/message \
		# http/src/URI

srcs := $(foreach dir, $(dirs), $(wildcard $(dir)/*.cpp))
# For my own testing purposes
srcs += main.cpp
include_dir := include/

includes := -I include/ \
			-I http/include/ \
			-I http/include/headers \
			-I http/include/message \
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
	make clean -C http/
	$(RM) -r $(obj_dir)

fclean: clean
	$(RM) $(NAME) http/libhttp.a

re: fclean all

optimized: CXXFLAGS += -O3
optimized: all

debug_server: CXXFLAGS += -g3
debug_server: all

fsan: CXXFLAGS += -fsanitize=address,undefined -g3
fsan: all

.PHONY: all clean fclean re optimized debug fsan
