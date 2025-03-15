# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/16 16:45:20 by kecheong          #+#    #+#              #
#    Updated: 2025/02/23 23:02:29 by kecheong         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := debug_server

NAME := webserv

CXX := g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -g3 #-fuse-ld=mold

src_dir := src
dirs := $(src_dir) \
		$(src_dir)/config \
		$(src_dir)/server \
		$(src_dir)/methods \
		$(src_dir)/URI \
		$(src_dir)/message \
		$(src_dir)/headers \
		$(src_dir)/utils \
		$(src_dir)/debug

srcs := $(foreach dir, $(dirs), $(wildcard $(dir)/*.cpp))
# For my own testing purposes
srcs += main.cpp
inc_dir := include

includes := -I $(inc_dir)/ \
			-I $(inc_dir)/config \
			-I $(inc_dir)/URI \
			-I $(inc_dir)/message \
			-I $(inc_dir)/headers \
			-I $(inc_dir)/utils \

obj_dir := obj
objs := $(srcs:$(src_dir)/%.cpp=$(obj_dir)/%.o)

uploads_dir := uploads

all: $(NAME)

$(NAME): $(objs) $(uploads_dir)
	$(CXX) $(CXXFLAGS) $(includes) $(objs) -o $(NAME)

$(obj_dir):
	mkdir -p $(obj_dir)

$(obj_dir)/%.o: $(src_dir)/%.cpp | obj
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(includes) $< -c -o $@

$(uploads_dir):
	mkdir -p $(uploads_dir)

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

lib:
	ar -rcs libserv.a $(objs)

.PHONY: all clean fclean re optimized debug fsan
