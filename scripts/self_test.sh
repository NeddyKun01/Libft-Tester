#!/usr/bin/env bash

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTER_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
TMP_ROOT="$(mktemp -d)"
FAILURES=0

cleanup()
{
	rm -rf "$TMP_ROOT"
}

trap cleanup EXIT

pass()
{
	printf "[OK]   %s\n" "$1"
}

fail()
{
	FAILURES=$((FAILURES + 1))
	printf "[NOK]  %s\n" "$1"
}

write_partial_libft()
{
	local root="$1"

	mkdir -p "$root"
	cp "${TESTER_DIR}/templates/libft.h" "$root/libft.h"
	cat >"$root/Makefile" <<'EOF'
NAME=libft.a
CC=cc
CFLAGS=-Wall -Wextra -Werror
AR=ar rcs
RM=rm -f
SRCS=ft_strlen.c
OBJS=$(SRCS:.c=.o)
all: $(NAME)
$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)
%.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	$(RM) $(OBJS)
fclean: clean
	$(RM) $(NAME)
re: fclean all
.PHONY: all clean fclean re
EOF
	cat >"$root/ft_strlen.c" <<'EOF'
#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
EOF
}

write_broken_makefile_libft()
{
	local root="$1"

	mkdir -p "$root"
	cat >"$root/Makefile" <<'EOF'
NAME=libft.a

all:
	@echo "intentional broken Makefile"
	@exit 2

clean:

fclean: clean

re: fclean all

.PHONY: all clean fclean re
EOF
	cat >"$root/libft.h" <<'EOF'
#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

size_t	ft_strlen(const char *s);

#endif
EOF
	cat >"$root/ft_strlen.c" <<'EOF'
#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
EOF
}

assert_exit()
{
	local expected="$1"
	local label="$2"
	shift 2

	"$@" >/tmp/libft_tester_self_test.out 2>&1
	local status=$?
	if [ "$expected" = "success" ] && [ "$status" -eq 0 ]; then
		pass "$label"
	elif [ "$expected" = "failure" ] && [ "$status" -ne 0 ]; then
		pass "$label"
	else
		fail "$label"
		sed -n '1,20p' /tmp/libft_tester_self_test.out | sed 's/^/       /'
	fi
}

assert_output_contains()
{
	local needle="$1"
	local label="$2"
	local file="$3"

	if grep -Fq "$needle" "$file"; then
		pass "$label"
	else
		fail "$label"
		printf "       missing: %s\n" "$needle"
		sed -n '1,30p' "$file" | sed 's/^/       /'
	fi
}

test_missing_header()
{
	local root="${TMP_ROOT}/missing_header"

	mkdir -p "$root"
	cp "${TESTER_DIR}/templates/Makefile" "$root/Makefile"
	assert_exit failure "diagnose fails when libft.h is missing" \
		bash "${TESTER_DIR}/scripts/diagnose.sh" "$root"
	assert_output_contains "libft.h missing" \
		"diagnose explains missing header" /tmp/libft_tester_self_test.out
}

test_partial_libft()
{
	local root="${TMP_ROOT}/partial_libft"
	local diagnose_log="${TMP_ROOT}/partial_diagnose.log"
	local rescue_log="${TMP_ROOT}/partial_rescue.log"
	local smart_log="${TMP_ROOT}/partial_smart.log"

	write_partial_libft "$root"
	if bash "${TESTER_DIR}/scripts/diagnose.sh" "$root" \
		>"$diagnose_log" 2>&1; then
		fail "diagnose fails for partial libft"
	else
		pass "diagnose fails for partial libft"
	fi
	assert_output_contains "ft_strlen         yes     yes     yes     OK" \
		"diagnose sees existing ft_strlen" "$diagnose_log"
	assert_output_contains "ft_split          no      yes     no" \
		"diagnose reports missing ft_split source/symbol" "$diagnose_log"
	if bash "${TESTER_DIR}/scripts/rescue_test.sh" "$root" --seed 42 \
		>"$rescue_log" 2>&1; then
		pass "rescue-test succeeds for partial libft"
	else
		fail "rescue-test succeeds for partial libft"
		sed -n '1,40p' "$rescue_log" | sed 's/^/       /'
	fi
	assert_output_contains "ft_strlen         OK" \
		"rescue-test runs existing ft_strlen" "$rescue_log"
	assert_output_contains "ft_split          SKIP missing symbol" \
		"rescue-test skips missing ft_split" "$rescue_log"
	if bash "${TESTER_DIR}/scripts/smart_run.sh" "$root" --seed 42 \
		>"$smart_log" 2>&1; then
		fail "smart run fails when Libft is structurally incomplete"
	else
		pass "smart run fails when Libft is structurally incomplete"
	fi
	assert_output_contains "Final Health Summary" \
		"smart run prints final health summary" "$smart_log"
	assert_output_contains "Diagnose:     PROBLEMS FOUND" \
		"smart run reports diagnose problems" "$smart_log"
	assert_output_contains "Rescue:       OK" \
		"smart run still runs valid rescue checks" "$smart_log"
}

test_broken_makefile()
{
	local root="${TMP_ROOT}/broken_makefile"
	local smart_log="${TMP_ROOT}/broken_makefile_smart.log"
	local menu_log="${TMP_ROOT}/broken_makefile_menu.log"

	write_broken_makefile_libft "$root"
	if bash "${TESTER_DIR}/scripts/smart_run.sh" "$root" --seed 42 \
		>"$smart_log" 2>&1; then
		fail "smart run fails when Makefile cannot build libft.a"
	else
		pass "smart run fails when Makefile cannot build libft.a"
	fi
	assert_output_contains "make failed" \
		"smart run shows Makefile build failure" "$smart_log"
	assert_output_contains "Rescue:       SKIPPED, libft.a was not available" \
		"smart run skips rescue when archive is missing" "$smart_log"
	assert_output_contains "Final Health Summary" \
		"smart run summarizes broken Makefile case" "$smart_log"
	if bash "${TESTER_DIR}/scripts/menu.sh" "$root" --seed 42 \
		>"$menu_log" 2>&1; then
		fail "menu falls back to smart run without TTY"
	else
		pass "menu falls back to smart run without TTY"
	fi
	assert_output_contains "Libft Tester Smart Run" \
		"menu non-interactive fallback prints smart run" "$menu_log"
}

printf "\nLibft Tester Self-Test\n"
printf "tmp: %s\n\n" "$TMP_ROOT"
test_missing_header
test_partial_libft
test_broken_makefile
printf "\nSelf-test failures: %d\n" "$FAILURES"

if [ "$FAILURES" -gt 0 ]; then
	exit 1
fi
exit 0
