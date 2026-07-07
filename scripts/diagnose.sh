#!/usr/bin/env bash

set -u

ROOT_DIR="${1:-..}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTER_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
MODEL_HEADER="${TESTER_DIR}/templates/libft.h"
MODEL_MAKEFILE="${TESTER_DIR}/templates/Makefile"
HEADER="${ROOT_DIR}/libft.h"
MAKEFILE="${ROOT_DIR}/Makefile"
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

declare -A PROTOTYPES=(
	[ft_isalpha]="int ft_isalpha(int c);"
	[ft_isdigit]="int ft_isdigit(int c);"
	[ft_isalnum]="int ft_isalnum(int c);"
	[ft_isascii]="int ft_isascii(int c);"
	[ft_isprint]="int ft_isprint(int c);"
	[ft_strlen]="size_t ft_strlen(const char *s);"
	[ft_memset]="void *ft_memset(void *s, int c, size_t n);"
	[ft_bzero]="void ft_bzero(void *s, size_t n);"
	[ft_memcpy]="void *ft_memcpy(void *dst, const void *src, size_t n);"
	[ft_memmove]="void *ft_memmove(void *dst, const void *src, size_t len);"
	[ft_strlcpy]="size_t ft_strlcpy(char *dst, const char *src, size_t dstsize);"
	[ft_strlcat]="size_t ft_strlcat(char *dst, const char *src, size_t dstsize);"
	[ft_toupper]="int ft_toupper(int c);"
	[ft_tolower]="int ft_tolower(int c);"
	[ft_strchr]="char *ft_strchr(const char *s, int c);"
	[ft_strrchr]="char *ft_strrchr(const char *s, int c);"
	[ft_strncmp]="int ft_strncmp(const char *s1, const char *s2, size_t n);"
	[ft_memchr]="void *ft_memchr(const void *s, int c, size_t n);"
	[ft_memcmp]="int ft_memcmp(const void *s1, const void *s2, size_t n);"
	[ft_strnstr]="char *ft_strnstr(const char *big, const char *little, size_t len);"
	[ft_atoi]="int ft_atoi(const char *nptr);"
	[ft_calloc]="void *ft_calloc(size_t count, size_t size);"
	[ft_strdup]="char *ft_strdup(const char *s);"
	[ft_substr]="char *ft_substr(char const *s, unsigned int start, size_t len);"
	[ft_strjoin]="char *ft_strjoin(char const *s1, char const *s2);"
	[ft_strtrim]="char *ft_strtrim(char const *s1, char const *set);"
	[ft_split]="char **ft_split(char const *s, char c);"
	[ft_itoa]="char *ft_itoa(int n);"
	[ft_strmapi]="char *ft_strmapi(char const *s, char (*f)(unsigned int, char));"
	[ft_striteri]="void ft_striteri(char *s, void (*f)(unsigned int, char *));"
	[ft_putchar_fd]="void ft_putchar_fd(char c, int fd);"
	[ft_putstr_fd]="void ft_putstr_fd(char *s, int fd);"
	[ft_putendl_fd]="void ft_putendl_fd(char *s, int fd);"
	[ft_putnbr_fd]="void ft_putnbr_fd(int n, int fd);"
	[ft_lstnew]="t_list *ft_lstnew(void *content);"
	[ft_lstadd_front]="void ft_lstadd_front(t_list **lst, t_list *new);"
	[ft_lstsize]="int ft_lstsize(t_list *lst);"
	[ft_lstlast]="t_list *ft_lstlast(t_list *lst);"
	[ft_lstadd_back]="void ft_lstadd_back(t_list **lst, t_list *new);"
	[ft_lstdelone]="void ft_lstdelone(t_list *lst, void (*del)(void *));"
	[ft_lstclear]="void ft_lstclear(t_list **lst, void (*del)(void *));"
	[ft_lstiter]="void ft_lstiter(t_list *lst, void (*f)(void *));"
	[ft_lstmap]="t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));"
)

ok_count=0
warning_count=0
error_count=0
missing_symbol_count=0
missing_header_count=0
missing_source_count=0

status_line()
{
	printf "%-11s %s\n" "$1" "$2"
}

warn()
{
	warning_count=$((warning_count + 1))
	status_line "[WARN]" "$1"
}

error()
{
	error_count=$((error_count + 1))
	status_line "[ERROR]" "$1"
}

ok()
{
	ok_count=$((ok_count + 1))
	status_line "[OK]" "$1"
}

has_header_decl()
{
	local name="$1"

	[ -f "$HEADER" ] || return 1
	grep -Eq "(^|[^A-Za-z0-9_])${name}[[:space:]]*\\(" "$HEADER"
}

has_symbol()
{
	local name="$1"

	[ -f "$LIBFT" ] || return 1
	nm -g "$LIBFT" 2>/dev/null | awk '{print $NF}' | grep -Fxq "$name"
}

has_source()
{
	local name="$1"

	[ -f "${ROOT_DIR}/${name}.c" ]
}

print_header()
{
	printf "\nLibft Tester Diagnose\n"
	printf "root: %s\n" "$ROOT_DIR"
	printf "model header: %s\n" "$MODEL_HEADER"
	printf "model makefile: %s\n\n" "$MODEL_MAKEFILE"
}

check_structure()
{
	if [ -d "$ROOT_DIR" ]; then
		ok "ROOT_DIR exists"
	else
		error "ROOT_DIR does not exist: $ROOT_DIR"
		return
	fi
	if [ -f "$MAKEFILE" ]; then
		ok "Makefile found"
	else
		error "Makefile missing"
		printf "           Fix: compare with %s\n" "$MODEL_MAKEFILE"
	fi
	if [ -f "$HEADER" ]; then
		ok "libft.h found"
	else
		error "libft.h missing"
		printf "           Fix: compare with %s\n" "$MODEL_HEADER"
	fi
}

check_build()
{
	local log_file
	local status

	printf "\nBuild check\n"
	if [ ! -f "$MAKEFILE" ]; then
		warn "Skipping make because Makefile is missing"
		return
	fi
	log_file="$(mktemp)"
	make -s -C "$ROOT_DIR" >"$log_file" 2>&1
	status=$?
	if [ "$status" -eq 0 ]; then
		ok "make completed successfully"
	else
		error "make failed"
		printf "           First build messages:\n"
		sed -n '1,12p' "$log_file" | sed 's/^/           /'
	fi
	rm -f "$log_file"
	if [ -f "$LIBFT" ]; then
		ok "libft.a found"
	else
		error "libft.a missing after make"
		printf "           Fix: Makefile must build a library named libft.a\n"
	fi
}

check_header_shape()
{
	printf "\nHeader shape\n"
	if [ ! -f "$HEADER" ]; then
		warn "Skipping header checks because libft.h is missing"
		return
	fi
	if grep -Eq "typedef[[:space:]]+struct[[:space:]]+s_list|t_list" "$HEADER"; then
		ok "t_list appears to be declared"
	else
		warn "t_list declaration not found"
		printf "           Fix: add the t_list struct used by list functions\n"
	fi
	if grep -Eq "#[[:space:]]*ifndef|#[[:space:]]*pragma[[:space:]]+once" "$HEADER"; then
		ok "include guard or pragma once found"
	else
		warn "include guard not found"
		printf "           Fix: add a normal include guard to libft.h\n"
	fi
}

function_status()
{
	local name="$1"
	local source="no"
	local header="no"
	local symbol="no"
	local status="OK"
	local fix="-"

	if has_source "$name"; then
		source="yes"
	else
		status="MISSING_SOURCE"
		fix="create ${name}.c"
		missing_source_count=$((missing_source_count + 1))
	fi
	if has_header_decl "$name"; then
		header="yes"
	else
		[ "$status" = "OK" ] && status="MISSING_HEADER"
		[ "$fix" = "-" ] && fix="add prototype: ${PROTOTYPES[$name]}"
		missing_header_count=$((missing_header_count + 1))
	fi
	if has_symbol "$name"; then
		symbol="yes"
	else
		[ "$status" = "OK" ] && status="MISSING_SYMBOL"
		if [ "$fix" = "-" ]; then
			if [ "$source" = "yes" ]; then
				fix="add ${name}.c to SRCS/Makefile"
			else
				fix="implement ${name}.c and build it"
			fi
		fi
		missing_symbol_count=$((missing_symbol_count + 1))
	fi
	printf "%-17s %-7s %-7s %-7s %-16s %s\n" \
		"$name" "$source" "$header" "$symbol" "$status" "$fix"
}

check_functions()
{
	local name

	printf "\nFunction matrix\n"
	printf "%-17s %-7s %-7s %-7s %-16s %s\n" \
		"Function" "Source" "Header" "Symbol" "Status" "Suggested fix"
	printf "%-17s %-7s %-7s %-7s %-16s %s\n" \
		"--------" "------" "------" "------" "------" "-------------"
	for name in "${EXPECTED_FUNCTIONS[@]}"; do
		function_status "$name"
	done
}

print_summary()
{
	printf "\nSummary\n"
	printf "structure/build errors: %d\n" "$error_count"
	printf "structure/build warnings: %d\n" "$warning_count"
	printf "missing source files: %d\n" "$missing_source_count"
	printf "missing header declarations: %d\n" "$missing_header_count"
	printf "missing library symbols: %d\n" "$missing_symbol_count"
	printf "\nRecommended next step\n"
	if [ "$error_count" -eq 0 ] \
		&& [ "$missing_header_count" -eq 0 ] \
		&& [ "$missing_symbol_count" -eq 0 ]; then
		printf "Run the normal tester: make ROOT_DIR=%s\n" "$ROOT_DIR"
	else
		printf "Fix the reported Makefile/header/source issues first.\n"
		printf "Use the known-good models only as references:\n"
		printf "- %s\n" "$MODEL_HEADER"
		printf "- %s\n" "$MODEL_MAKEFILE"
	fi
}

print_header
check_structure
check_build
check_header_shape
check_functions
print_summary

if [ "$error_count" -gt 0 ] \
	|| [ "$missing_source_count" -gt 0 ] \
	|| [ "$missing_header_count" -gt 0 ] \
	|| [ "$missing_symbol_count" -gt 0 ]; then
	exit 1
fi
exit 0
