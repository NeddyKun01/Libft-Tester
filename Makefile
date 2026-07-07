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

# PT: Compilador C++ usado no tester.
# EN: C++ compiler used by the tester.
CXX=c++
CXXFLAGS=-std=c++17 -Wall -Wextra -Werror
LDFLAGS=-Wl,--wrap=malloc

.DEFAULT_GOAL := all
MAKEFLAGS += --no-print-directory

# PT: Caminhos base do projeto e do tester.
# EN: Base paths for the project and the tester.
ROOT_DIR=..
SRC_DIR=src
INC_DIR=include
BUILD_DIR=build
NAME=libft_tester
LIBFT=$(ROOT_DIR)/libft.a
ARGS=
REPORT=libft-test-report.json
HTML_REPORT=libft-test-report.html

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
HEADERS=$(INC_DIR)/coverage.hpp \
		$(INC_DIR)/hints.hpp \
		$(INC_DIR)/malloc_fail.hpp \
		$(INC_DIR)/profiles.hpp \
		$(INC_DIR)/test_modules.hpp \
		$(INC_DIR)/tester.hpp

# PT: Verifica se ROOT_DIR aponta para uma Libft valida.
# EN: Checks whether ROOT_DIR points to a valid Libft project.
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

# PT: Garante que a biblioteca principal existe antes de ligar o tester.
# EN: Ensures the main library exists before linking the tester.
$(LIBFT): | check-root
	@$(MAKE) -s -C $(ROOT_DIR)

# PT: Cria a pasta onde ficam os objetos do tester.
# EN: Creates the folder where the tester objects are stored.
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# PT: Compila um objeto C++ a partir do ficheiro fonte correspondente.
# EN: Compiles one C++ object from its matching source file.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) $(LIBFT) | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -I$(INC_DIR) -I$(ROOT_DIR) -c $< -o $@

# PT: Liga o executavel final do tester.
#     O wrap do malloc deixa o tester simular falhas de alocacao.
# EN: Links the final tester executable.
#     The malloc wrap lets the tester simulate allocation failures.
$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $^ $(LIBFT) $(LDFLAGS) -o $@

# PT: Compila o tester e corre a suite toda.
# EN: Builds the tester and runs the full suite.
all: $(NAME)
	@./$(NAME) $(ARGS)

# PT: Mostra a ajuda do tester.
# EN: Shows the tester help message.
help: $(NAME)
	@./$(NAME) --help

# PT: Corre a suite sem recompilar se o binario ja existir.
# EN: Runs the suite without recompiling if the binary already exists.
run: $(NAME)
	@./$(NAME) $(ARGS)

# PT: Mostra as suites e funcoes conhecidas.
# EN: Shows the known suites and functions.
list: $(NAME)
	@./$(NAME) --list

# PT: Mostra os perfis de execucao disponiveis.
# EN: Shows the available run profiles.
profiles: $(NAME)
	@./$(NAME) --profiles

# PT: Diagnostica Makefile, header, sources e simbolos sem compilar o runner.
# EN: Diagnoses Makefile, header, sources, and symbols without building runner.
diagnose:
	@bash scripts/diagnose.sh "$(ROOT_DIR)"

# PT: Testa apenas funcoes com simbolos reais, usando header modelo e stubs.
# EN: Tests only real symbols, using the model header and weak stubs.
rescue-test:
	@bash scripts/rescue_test.sh "$(ROOT_DIR)" $(ARGS)

# PT: Mostra a tabela de cobertura documentada.
# EN: Shows the documented coverage table.
coverage: $(NAME)
	@./$(NAME) --coverage

# PT: Atualiza a documentacao de cobertura a partir do codigo.
# EN: Updates coverage documentation from the code metadata.
coverage-docs: $(NAME)
	@./$(NAME) --coverage-md > docs/COVERAGE.md
	@printf "Coverage documentation updated: docs/COVERAGE.md\n"

# PT: Explica os casos testados para uma funcao. Usa FUNC=ft_split.
# EN: Explains the cases tested for one function. Use FUNC=ft_split.
explain: $(NAME)
	@./$(NAME) --explain $(FUNC)

# PT: Mostra dicas de debugging para uma funcao. Usa FUNC=ft_split.
# EN: Shows debugging hints for one function. Use FUNC=ft_split.
hint: $(NAME)
	@./$(NAME) --hint $(FUNC)

# PT: Corre sem agregar os estados dos testes.
# EN: Runs without aggregating test statuses.
verbose: $(NAME)
	@./$(NAME) --verbose $(ARGS)

# PT: Produz output JSON para scripts e CI.
# EN: Produces JSON output for scripts and CI.
json: $(NAME)
	@./$(NAME) --json $(ARGS)

# PT: Output estavel para CI: JSON e sem cores.
# EN: Stable CI output: JSON and no colors.
ci: $(NAME)
	@./$(NAME) --json --no-color $(ARGS)

# PT: Mostra apenas o resumo final.
# EN: Shows only the final summary.
summary: $(NAME)
	@./$(NAME) --summary-only $(ARGS)

# PT: Corre um perfil rapido para feedback durante desenvolvimento.
# EN: Runs a quick profile for development feedback.
quick: $(NAME)
	@./$(NAME) --profile quick $(ARGS)

# PT: Corre o perfil strict para validacao mais forte.
# EN: Runs the strict profile for stronger validation.
strict: $(NAME)
	@./$(NAME) --profile strict $(ARGS)

# PT: Corre o perfil brutal antes de push/release.
# EN: Runs the brutal profile before push/release.
brutal: $(NAME)
	@./$(NAME) --profile brutal $(ARGS)

# PT: Guarda output JSON num ficheiro para CI/artifacts.
# EN: Saves JSON output to a file for CI/artifacts.
report: $(NAME)
	@./$(NAME) --json --no-color $(ARGS) > $(REPORT)
	@printf "Report written to %s\n" "$(REPORT)"

# PT: Guarda um relatorio HTML standalone.
# EN: Saves a standalone HTML report.
report-html: $(NAME)
	@./$(NAME) --html --no-color $(ARGS) > $(HTML_REPORT)
	@printf "HTML report written to %s\n" "$(HTML_REPORT)"

# PT: Corre com Valgrind para apanhar leaks e acessos invalidos.
# EN: Runs with Valgrind to catch leaks and invalid memory accesses.
leaks: $(NAME)
	@LIBFT_TESTER_NO_FORK=1 valgrind --leak-check=full \
		--show-leak-kinds=all --track-origins=yes \
		--errors-for-leak-kinds=all --error-exitcode=42 ./$(NAME) $(ARGS); \
		exit $$?

# PT: Remove os objetos e o executavel do tester.
# EN: Removes the tester objects and executable.
clean:
	@rm -rf $(BUILD_DIR) $(NAME) $(REPORT) $(HTML_REPORT)

# PT: Remove tudo o que o tester gerou.
# EN: Removes everything generated by the tester.
fclean: clean
	@$(MAKE) -s -C $(ROOT_DIR) fclean

# PT: Reconstroi tudo do zero.
# EN: Rebuilds everything from scratch.
re: fclean all

# PT: Alvos que nao criam ficheiros.
# EN: Targets that do not create files.
.PHONY: all check-root help run list profiles diagnose rescue-test coverage coverage-docs explain hint verbose json ci summary quick strict brutal report report-html leaks clean fclean re
