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

NAME := webserv
BUILD := $(NAME)
DEBUG_BUILD := $(BUILD)_debug
FSAN_BUILD := $(BUILD)_fsan
RELEASE_BUILD := $(BUILD)_release

CXX := c++
CXXFLAGS = -Wall -Werror -Wextra -MMD -std=c++98 -pedantic

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

build_obj_dir := $(obj_dir)/build
build_obj_dirs := $(subst $(src_dir), $(build_obj_dir), $(dirs))
build_objs := $(srcs:$(src_dir)/%.cpp=$(build_obj_dir)/%.o)
build_deps := $(build_objs:%.o=%.d);

fsan_obj_dir := $(obj_dir)/fsanitize
fsan_obj_dirs := $(subst $(src_dir), $(fsan_obj_dir), $(dirs))
fsan_objs := $(srcs:$(src_dir)/%.cpp=$(fsan_obj_dir)/%.o)
fsan_deps := $(fsan_objs:%.o=%.d)

debug_obj_dir := $(obj_dir)/debug
debug_obj_dirs := $(subst $(src_dir), $(debug_obj_dir), $(dirs))
debug_objs := $(srcs:$(src_dir)/%.cpp=$(debug_obj_dir)/%.o)
debug_deps := $(debug_objs:%.o=%.d)

release_obj_dir := $(obj_dir)/release
release_obj_dirs := $(subst $(src_dir), $(release_obj_dir), $(dirs))
release_objs := $(srcs:$(src_dir)/%.cpp=$(release_obj_dir)/%.o)
release_deps := $(release_objs:%.o=%.d)

all: $(NAME)

$(NAME): $(build_objs)
	$(CXX) $(CXXFLAGS) $(includes) $(build_objs) -o $(NAME)

$(build_obj_dir):
	mkdir -p $(build_obj_dirs)

$(build_obj_dir)/%.o: $(src_dir)/%.cpp | $(build_obj_dir)
	$(CXX) $(CXXFLAGS) $(includes) $< -c -o $@

debug: CXXFLAGS += -O0 -g3
debug: $(DEBUG_BUILD)

$(DEBUG_BUILD): $(debug_objs)
	$(CXX) $(CXXFLAGS) $(includes) $(debug_objs) -o $(DEBUG_BUILD)

$(debug_obj_dir)/%.o: $(src_dir)/%.cpp | $(debug_obj_dir)
	$(CXX) $(CXXFLAGS) $(includes) $< -c -o $@

$(debug_obj_dir):
	mkdir -p $(debug_obj_dirs)

fsan: CXXFLAGS += -fsanitize=address,undefined -g3
fsan: $(FSAN_BUILD)

$(FSAN_BUILD): $(fsan_objs)
	$(CXX) $(CXXFLAGS) $(includes) $(fsan_objs) -o $(FSAN_BUILD)

$(fsan_obj_dir)/%.o: $(src_dir)/%.cpp | $(fsan_obj_dir)
	$(CXX) $(CXXFLAGS) $(includes) $< -c -o $@

$(fsan_obj_dir):
	mkdir -p $(fsan_obj_dirs)

release: CXXFLAGS += -O3
release: $(RELEASE_BUILD)

$(RELEASE_BUILD): $(release_objs)
	$(CXX) $(CXXFLAGS) $(includes) $(release_objs) -o $(RELEASE_BUILD)

$(release_obj_dir)/%.o: $(src_dir)/%.cpp | $(release_obj_dir)
	$(CXX) $(CXXFLAGS) $(includes) $< -c -o $@

$(release_obj_dir):
	mkdir -p $(release_obj_dirs)

lib:
	ar -rcs libserv.a $(objs)

clean:
	$(RM) -r $(obj_dir)

fclean: clean
	$(RM) $(NAME) $(DEBUG_BUILD) $(FSAN_BUILD)

re: fclean all

.PHONY: all clean fclean re release debug fsan

-include $(build_deps)
-include $(release_deps)
-include $(debug_deps)
-include $(fsan_deps)
