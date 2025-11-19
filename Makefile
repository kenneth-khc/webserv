# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/16 16:45:20 by kecheong          #+#    #+#              #
#    Updated: 2025/11/20 05:15:02 by cteoh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := debug

NAME := webserv
DEBUG_BUILD := webserv_debug
FSAN_BUILD := webserv_fsan
RELEASE_BUILD := webserv_release

CXX := c++
CXXFLAGS = -Wall -Werror -Wextra -MMD -std=c++98

src_dir := src
dirs := $(src_dir) \
		$(src_dir)/config \
		$(src_dir)/config/errors \
		$(src_dir)/server \
		$(src_dir)/client \
		$(src_dir)/server/errors \
		$(src_dir)/methods \
		$(src_dir)/URI \
		$(src_dir)/message \
		$(src_dir)/message/request \
		$(src_dir)/headers \
		$(src_dir)/utils \
		$(src_dir)/utils/Time \
		$(src_dir)/CGI \
		$(src_dir)/debug

srcs := $(foreach dir, $(dirs), $(wildcard $(dir)/*.cpp))
inc_dir := include

includes := -I $(inc_dir)/ \
			-I $(inc_dir)/config \
			-I $(inc_dir)/config/errors \
			-I $(inc_dir)/client \
			-I $(inc_dir)/URI \
			-I $(inc_dir)/message \
			-I $(inc_dir)/message/request \
			-I $(inc_dir)/headers \
			-I $(inc_dir)/utils \
			-I $(inc_dir)/utils/Time \
			-I $(inc_dir)/CGI \
			-I $(inc_dir)/server \
			-I $(inc_dir)/server/errors

obj_dir := obj
obj_dirs := $(subst $(src_dir), $(obj_dir), $(dirs))
objs := $(srcs:$(src_dir)/%.cpp=$(obj_dir)/%.o)
deps := $(objs:%.o=%.d)

all: $(NAME)

$(NAME): $(objs)
	$(CXX) $(CXXFLAGS) $(includes) $(objs) -o $(NAME)

$(obj_dir):
	mkdir -p $(obj_dirs)

$(obj_dir)/%.o: $(src_dir)/%.cpp | $(obj_dir)
	$(CXX) $(CXXFLAGS) $(includes) $< -c -o $@

clean:
	$(RM) -r $(obj_dir)

fclean: clean
	$(RM) $(NAME) $(DEBUG_BUILD)

re: fclean all

debug_server: CXXFLAGS += -O0 -g3
debug_server: all

debug: CXXFLAGS += -O0 -g3
debug: $(DEBUG_BUILD)

$(DEBUG_BUILD): $(objs)
	$(CXX) $(CXXFLAGS) $(includes) $(objs) -o $(DEBUG_BUILD)

fsan: CXXFLAGS += -fsanitize=address,undefined -g3
fsan: $(FSAN_BUILD)

$(FSAN_BUILD): $(objs)
	$(CXX) $(CXXFLAGS) $(includes) $(objs) -o $(FSAN_BUILD)

release: CXXFLAGS += -O3
release: $(RELEASE_BUILD)

$(RELEASE_BUILD): $(objs)
	$(CXX) $(CXXFLAGS) $(includes) $(objs) -o $(RELEASE_BUILD)

lib:
	ar -rcs libserv.a $(objs)

.PHONY: all clean fclean re optimized debug fsan

-include $(deps)
