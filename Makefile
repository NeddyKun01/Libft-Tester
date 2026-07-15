# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: libft-tester <opensource@example.com>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/06 11:02:52 by libft-tester          #+#    #+#              #
#    Updated: 2026/07/08 14:00:00 by libft-tester         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX=c++
CC=cc
CXXFLAGS=-std=c++17 -Wall -Wextra -Werror
CFLAGS=-Wall -Wextra -Werror
LDFLAGS=-Wl,--wrap=malloc

.DEFAULT_GOAL := all
MAKEFLAGS += --no-print-directory

ROOT_DIR=..
ARGS=

NAME=libft_tester
LIBFT=$(ROOT_DIR)/libft.a

CODE_DIR=tester
DRIVER_DIR=$(CODE_DIR)/driver
SRC_DIR=$(CODE_DIR)/src
TEST_DIR=$(CODE_DIR)/tests
INC_DIR=$(CODE_DIR)/include
RESCUE_DIR=$(CODE_DIR)/rescue
TEMPLATE_DIR=$(CODE_DIR)/templates
ifeq ($(LIBFT_TESTER_SELF_TEST),1)
BUILD_DIR=$(CODE_DIR)/build-self-test
else
BUILD_DIR=$(CODE_DIR)/build
endif
DRIVER_BUILD=$(BUILD_DIR)/driver
SUITE_BUILD=$(BUILD_DIR)/suite
TEST_BUILD=$(BUILD_DIR)/tests
RESCUE_BUILD=$(BUILD_DIR)/rescue
ROOT_STAMP=$(BUILD_DIR)/root.stamp

SUITE_NAME=$(BUILD_DIR)/libft_suite
RESCUE_NAME=$(RESCUE_BUILD)/libft_suite_rescue

DRIVER_SRCS=$(wildcard $(DRIVER_DIR)/*.cpp)
DRIVER_OBJS=$(DRIVER_SRCS:$(DRIVER_DIR)/%.cpp=$(DRIVER_BUILD)/%.o)
DRIVER_INFO_SRCS=$(SRC_DIR)/coverage.cpp $(SRC_DIR)/hints.cpp
DRIVER_INFO_OBJS=$(DRIVER_INFO_SRCS:$(SRC_DIR)/%.cpp=$(DRIVER_BUILD)/info_%.o)

SRC_SRCS=$(wildcard $(SRC_DIR)/*.cpp)
ALL_TEST_SRCS=$(filter-out $(TEST_DIR)/selftest_suites.cpp,$(wildcard $(TEST_DIR)/*.cpp))
SELF_TEST_SRCS=$(TEST_DIR)/selftest_suites.cpp \
		$(TEST_DIR)/ft_strlen.cpp \
		$(TEST_DIR)/ft_split.cpp
ifeq ($(LIBFT_TESTER_SELF_TEST),1)
TEST_SRCS=$(SELF_TEST_SRCS)
else
TEST_SRCS=$(ALL_TEST_SRCS)
endif
SUITE_OBJS=$(SRC_SRCS:$(SRC_DIR)/%.cpp=$(SUITE_BUILD)/src_%.o) \
		$(TEST_SRCS:$(TEST_DIR)/%.cpp=$(TEST_BUILD)/%.o)
RESCUE_OBJS=$(SRC_SRCS:$(SRC_DIR)/%.cpp=$(RESCUE_BUILD)/src_%.o) \
		$(TEST_SRCS:$(TEST_DIR)/%.cpp=$(RESCUE_BUILD)/test_%.o)

HEADERS=$(INC_DIR)/coverage.hpp \
		$(INC_DIR)/hints.hpp \
		$(INC_DIR)/malloc_fail.hpp \
		$(INC_DIR)/profiles.hpp \
		$(INC_DIR)/test_modules.hpp \
		$(INC_DIR)/tester.hpp

TEST_HEADERS=$(TEST_DIR)/test_helpers.hpp

all: build
	@./$(NAME) --root "$(ROOT_DIR)" $(ARGS)

menu: all

build: $(NAME)

suite: $(ROOT_STAMP) $(SUITE_NAME)

rescue-suite: $(ROOT_STAMP) $(RESCUE_NAME)

$(BUILD_DIR) $(DRIVER_BUILD) $(SUITE_BUILD) $(TEST_BUILD) $(RESCUE_BUILD):
	@mkdir -p $@

$(ROOT_STAMP): FORCE | $(BUILD_DIR)
	@old_root=""; \
	if [ -f "$(ROOT_STAMP)" ]; then \
		old_root=$$(cat "$(ROOT_STAMP)"); \
	fi; \
	if [ "$$old_root" != "$(ROOT_DIR)" ]; then \
		rm -rf $(SUITE_BUILD) $(TEST_BUILD) $(RESCUE_BUILD); \
		mkdir -p $(SUITE_BUILD) $(TEST_BUILD) $(RESCUE_BUILD); \
		printf "%s" "$(ROOT_DIR)" > "$(ROOT_STAMP)"; \
	fi

$(DRIVER_BUILD)/%.o: $(DRIVER_DIR)/%.cpp $(DRIVER_DIR)/driver.hpp | $(DRIVER_BUILD)
	@$(CXX) $(CXXFLAGS) -I$(DRIVER_DIR) -I$(INC_DIR) -c $< -o $@

$(DRIVER_BUILD)/info_%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(DRIVER_BUILD)
	@$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

$(NAME): $(DRIVER_OBJS) $(DRIVER_INFO_OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@

check-root:
	@if [ ! -d "$(ROOT_DIR)" ]; then \
		printf "Error: ROOT_DIR '%s' does not exist.\n" "$(ROOT_DIR)"; \
		printf "Use: make ROOT_DIR=/path/to/libft\n"; \
		exit 1; \
	fi

$(LIBFT): | check-root
	@$(MAKE) -s -C $(ROOT_DIR)

$(SUITE_BUILD)/src_%.o: $(SRC_DIR)/%.cpp $(HEADERS) $(LIBFT) $(ROOT_STAMP) | $(SUITE_BUILD)
	@$(CXX) $(CXXFLAGS) -I$(INC_DIR) -I$(ROOT_DIR) -c $< -o $@

$(TEST_BUILD)/%.o: $(TEST_DIR)/%.cpp $(HEADERS) $(TEST_HEADERS) $(LIBFT) $(ROOT_STAMP) | $(TEST_BUILD)
	@$(CXX) $(CXXFLAGS) -I$(TEST_DIR) -I$(INC_DIR) -I$(ROOT_DIR) -c $< -o $@

$(SUITE_NAME): $(ROOT_STAMP) $(SUITE_OBJS) $(LIBFT)
	@$(CXX) $(CXXFLAGS) $(SUITE_OBJS) $(LIBFT) $(LDFLAGS) -o $@

$(RESCUE_BUILD)/src_%.o: $(SRC_DIR)/%.cpp $(HEADERS) $(LIBFT) $(ROOT_STAMP) | $(RESCUE_BUILD)
	@$(CXX) $(CXXFLAGS) -I$(TEMPLATE_DIR) -I$(INC_DIR) -I$(ROOT_DIR) -c $< -o $@

$(RESCUE_BUILD)/test_%.o: $(TEST_DIR)/%.cpp $(HEADERS) $(TEST_HEADERS) $(LIBFT) $(ROOT_STAMP) | $(RESCUE_BUILD)
	@$(CXX) $(CXXFLAGS) -I$(TEMPLATE_DIR) -I$(TEST_DIR) -I$(INC_DIR) -I$(ROOT_DIR) -c $< -o $@

$(RESCUE_BUILD)/stubs.o: $(RESCUE_DIR)/stubs.c $(TEMPLATE_DIR)/libft.h $(ROOT_STAMP) | $(RESCUE_BUILD)
	@$(CC) $(CFLAGS) -I$(TEMPLATE_DIR) -c $< -o $@

$(RESCUE_NAME): $(ROOT_STAMP) $(RESCUE_OBJS) $(RESCUE_BUILD)/stubs.o $(LIBFT)
	@$(CXX) $(CXXFLAGS) $(RESCUE_OBJS) $(LDFLAGS) \
		-Wl,--whole-archive $(LIBFT) -Wl,--no-whole-archive \
		$(RESCUE_BUILD)/stubs.o -o $@

self-test: build
	@./$(NAME) --self-test

clean:
	@rm -rf $(BUILD_DIR) $(NAME) libft-test-report.json libft-test-report.html

fclean: clean

re: fclean build

FORCE:

.PHONY: all menu build suite rescue-suite check-root self-test clean fclean re FORCE
