#!/usr/bin/env bash

set -u

ROOT_DIR="${1:-..}"
shift || true
EXTRA_ARGS=("$@")
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTER_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${TESTER_DIR}/build/rescue"
RUNNER="${BUILD_DIR}/libft_tester_rescue"
LIBFT="${ROOT_DIR}/libft.a"

EXPECTED_FUNCTIONS=(
	ft_isalpha ft_isdigit ft_isalnum ft_isascii ft_isprint
	ft_strlen ft_memset ft_bzero ft_memcpy ft_memmove ft_strlcpy ft_strlcat
	ft_toupper ft_tolower ft_strchr ft_strrchr ft_strncmp ft_memchr ft_memcmp
	ft_strnstr ft_atoi ft_calloc ft_strdup ft_substr ft_strjoin ft_strtrim
	ft_split ft_itoa ft_strmapi ft_striteri ft_putchar_fd ft_putstr_fd
	ft_putendl_fd ft_putnbr_fd ft_lstnew ft_lstadd_front ft_lstsize
	ft_lstlast ft_lstadd_back ft_lstdelone ft_lstclear ft_lstiter ft_lstmap
)

declare -A SUITES=(
	[ft_isalpha]=ctype [ft_isdigit]=ctype [ft_isalnum]=ctype
	[ft_isascii]=ctype [ft_isprint]=ctype [ft_toupper]=ctype
	[ft_tolower]=ctype [ft_memset]=memory [ft_bzero]=memory
	[ft_memcpy]=memory [ft_memmove]=memory [ft_memchr]=memory
	[ft_memcmp]=memory [ft_calloc]=memory [ft_atoi]=atoi
	[ft_strlen]=strings [ft_strchr]=strings [ft_strrchr]=strings
	[ft_strncmp]=strings [ft_strnstr]=strings [ft_strlcpy]=strings
	[ft_strlcat]=strings [ft_strdup]=strings [ft_substr]=string_utils
	[ft_strjoin]=string_utils [ft_strtrim]=string_utils
	[ft_split]=string_utils [ft_itoa]=string_utils
	[ft_strmapi]=string_utils [ft_striteri]=string_utils
	[ft_putchar_fd]=output [ft_putstr_fd]=output
	[ft_putendl_fd]=output [ft_putnbr_fd]=output
	[ft_lstnew]=lists [ft_lstadd_front]=lists [ft_lstsize]=lists
	[ft_lstlast]=lists [ft_lstadd_back]=lists [ft_lstdelone]=lists
	[ft_lstclear]=lists [ft_lstiter]=lists [ft_lstmap]=lists
)

has_symbol()
{
	local name="$1"

	[ -f "$LIBFT" ] || return 1
	nm -g "$LIBFT" 2>/dev/null | awk '{print $NF}' | grep -Fxq "$name"
}

has_direct_rescue_checks()
{
	local name="$1"

	[ "$name" != "ft_lstdelone" ]
}

compile_runner()
{
	local src
	local obj
	local objects=()

	mkdir -p "$BUILD_DIR"
	for src in "${TESTER_DIR}"/src/*.cpp; do
		obj="${BUILD_DIR}/$(basename "${src%.cpp}.o")"
		c++ -std=c++17 -Wall -Wextra -Werror \
			-I"${TESTER_DIR}/templates" -I"${TESTER_DIR}/include" \
			-I"${ROOT_DIR}" -c "$src" -o "$obj" || return 1
		objects+=("$obj")
	done
	cc -Wall -Wextra -Werror -I"${TESTER_DIR}/templates" \
		-c "${TESTER_DIR}/rescue/stubs.c" -o "${BUILD_DIR}/stubs.o" \
		|| return 1
	c++ -std=c++17 -Wall -Wextra -Werror "${objects[@]}" \
		-Wl,--wrap=malloc -Wl,--whole-archive "$LIBFT" \
		-Wl,--no-whole-archive "${BUILD_DIR}/stubs.o" -o "$RUNNER"
}

run_function()
{
	local name="$1"
	local suite="${SUITES[$name]}"
	local log_file="${BUILD_DIR}/${name}.log"

	if "$RUNNER" --suite "$suite" --only "$name" --summary-only \
		--no-color "${EXTRA_ARGS[@]}" >"$log_file" 2>&1; then
		printf "%-17s OK\n" "$name"
		return 0
	fi
	printf "%-17s NOK see %s\n" "$name" "$log_file"
	return 1
}

main()
{
	local name
	local found=0
	local skipped_missing=0
	local skipped_indirect=0
	local failed=0

	printf "\nLibft Tester Rescue Test\n"
	printf "root: %s\n" "$ROOT_DIR"
	printf "runner: %s\n\n" "$RUNNER"
	if [ ! -f "$LIBFT" ]; then
		printf "[ERROR] libft.a not found. Run diagnose first.\n"
		exit 1
	fi
	if ! compile_runner; then
		printf "[ERROR] rescue runner failed to build.\n"
		exit 1
	fi
	printf "Function results\n"
	printf "%-17s %s\n" "Function" "Status"
	printf "%-17s %s\n" "--------" "------"
	for name in "${EXPECTED_FUNCTIONS[@]}"; do
		if has_symbol "$name"; then
			found=$((found + 1))
			if ! has_direct_rescue_checks "$name"; then
				skipped_indirect=$((skipped_indirect + 1))
				printf "%-17s SKIP indirect coverage\n" "$name"
			elif ! run_function "$name"; then
				failed=$((failed + 1))
			fi
		else
			skipped_missing=$((skipped_missing + 1))
			printf "%-17s SKIP missing symbol\n" "$name"
		fi
	done
	printf "\nSummary\n"
	printf "real symbols found: %d\n" "$found"
	printf "missing symbols skipped: %d\n" "$skipped_missing"
	printf "indirect-only skipped: %d\n" "$skipped_indirect"
	printf "failed tested functions: %d\n" "$failed"
	if [ "$failed" -gt 0 ]; then
		exit 1
	fi
	exit 0
}

main "$@"
