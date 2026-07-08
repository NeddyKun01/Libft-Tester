# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: neddykun <neddykun@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/06 11:02:52 by neddykun          #+#    #+#              #
#    Updated: 2026/07/06 11:02:52 by neddykun         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX=c++
CXXFLAGS=-std=c++17 -Wall -Wextra -Werror
LDFLAGS=-Wl,--wrap=malloc

.DEFAULT_GOAL := all
MAKEFLAGS += --no-print-directory

ROOT_DIR=..
SRC_DIR=src
INC_DIR=include
BUILD_DIR=build/obj
NAME=libft_tester
LIBFT=$(ROOT_DIR)/libft.a
ARGS=

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
HEADERS=$(INC_DIR)/coverage.hpp \
		$(INC_DIR)/hints.hpp \
		$(INC_DIR)/malloc_fail.hpp \
		$(INC_DIR)/profiles.hpp \
		$(INC_DIR)/test_modules.hpp \
		$(INC_DIR)/tester.hpp

all:
	@bash scripts/menu.sh "$(ROOT_DIR)" $(ARGS)

menu:
	@bash scripts/menu.sh "$(ROOT_DIR)" $(ARGS)

build: $(NAME)

check-root:
	@if [ ! -d "$(ROOT_DIR)" ]; then \
		printf "Error: ROOT_DIR '%s' does not exist.\n" "$(ROOT_DIR)"; \
		printf "Use: make ROOT_DIR=/path/to/libft\n"; \
		exit 1; \
	fi
	@if [ ! -f "$(ROOT_DIR)/Makefile" ]; then \
		printf "Error: '%s/Makefile' was not found.\n" "$(ROOT_DIR)"; \
		printf "Use: make ROOT_DIR=/path/to/libft\n"; \
		exit 1; \
	fi
	@if [ ! -f "$(ROOT_DIR)/libft.h" ]; then \
		printf "Error: '%s/libft.h' was not found.\n" "$(ROOT_DIR)"; \
		printf "Use: make ROOT_DIR=/path/to/libft\n"; \
		exit 1; \
	fi

$(LIBFT): | check-root
	@$(MAKE) -s -C $(ROOT_DIR)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) $(LIBFT) | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -I$(INC_DIR) -I$(ROOT_DIR) -c $< -o $@

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ $(LIBFT) $(LDFLAGS) -o $@

self-test:
	@bash scripts/self_test.sh

clean:
	@rm -rf build $(NAME) libft-test-report.json libft-test-report.html

fclean: clean

re: fclean build

.PHONY: all menu build check-root self-test clean fclean re
