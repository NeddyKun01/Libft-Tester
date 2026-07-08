#!/usr/bin/env bash

set -u

ROOT_DIR="${1:-..}"
shift || true
EXTRA_ARGS=("$@")
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTER_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
RUNNER="${TESTER_DIR}/libft_tester"
HTML_REPORT="${TESTER_DIR}/libft-test-report.html"

if [ ! -t 0 ] || [ ! -t 1 ]; then
	bash "${TESTER_DIR}/scripts/smart_run.sh" "$ROOT_DIR" "${EXTRA_ARGS[@]}"
	exit $?
fi

ok()
{
	printf "OK"
}

missing()
{
	printf "missing"
}

status_file()
{
	if [ -f "$1" ]; then
		ok
	else
		missing
	fi
}

pause()
{
	printf "\nPress Enter to return to the menu..."
	read -r _
}

build_runner()
{
	make -s -C "$TESTER_DIR" build ROOT_DIR="$ROOT_DIR"
}

run_runner()
{
	if build_runner; then
		"$RUNNER" "$@" "${EXTRA_ARGS[@]}"
	fi
}

run_leaks()
{
	if build_runner; then
		env LIBFT_TESTER_NO_FORK=1 valgrind --leak-check=full \
			--show-leak-kinds=all --track-origins=yes \
			--errors-for-leak-kinds=all --error-exitcode=42 \
			"$RUNNER" --only ft_split --no-color "${EXTRA_ARGS[@]}"
	fi
}

generate_html()
{
	if build_runner; then
		"$RUNNER" --html --no-color "${EXTRA_ARGS[@]}" > "$HTML_REPORT"
		printf "HTML report written to %s\n" "$HTML_REPORT"
	fi
}

run_and_pause()
{
	"$@"
	pause
}

print_header()
{
	clear 2>/dev/null || true
	printf "\n"
	printf "============================================================\n"
	printf " Libft Tester\n"
	printf "============================================================\n"
	printf " root:     %s\n" "$ROOT_DIR"
	printf " Makefile: %s\n" "$(status_file "${ROOT_DIR}/Makefile")"
	printf " libft.h:  %s\n" "$(status_file "${ROOT_DIR}/libft.h")"
	printf " libft.a:  %s\n" "$(status_file "${ROOT_DIR}/libft.a")"
	printf " runner:   %s\n" "$(status_file "$RUNNER")"
	printf "============================================================\n"
}

print_menu()
{
	printf "\n"
	printf "Recommended\n"
	printf "  1) Smart test\n"
	printf "  2) Quick test\n"
	printf "  3) Full test\n"
	printf "  4) Strict test\n"
	printf "\n"
	printf "Debugging\n"
	printf "  5) Diagnose project\n"
	printf "  6) Rescue test\n"
	printf "  7) Leak check\n"
	printf "  8) Explain or hint a function\n"
	printf "\n"
	printf "Reports and help\n"
	printf "  9) Generate HTML report\n"
	printf "  h) Advanced CLI help\n"
	printf "  r) Change ROOT_DIR\n"
	printf "  0) Exit\n"
	printf "\nChoice: "
}

function_tools()
{
	local name
	local choice

	printf "Function name, for example ft_split: "
	read -r name
	if [ -z "$name" ]; then
		return
	fi
	printf "\n"
	printf "  1) Explain tested cases\n"
	printf "  2) Show debugging hint\n"
	printf "\nChoice: "
	read -r choice
	case "$choice" in
		1) run_and_pause run_runner --explain "$name" ;;
		2) run_and_pause run_runner --hint "$name" ;;
		*) printf "Invalid choice.\n"; pause ;;
	esac
}

change_root()
{
	local new_root

	printf "New ROOT_DIR: "
	read -r new_root
	if [ -n "$new_root" ]; then
		ROOT_DIR="$new_root"
	fi
}

main_loop()
{
	local choice

	while true; do
		print_header
		print_menu
		read -r choice
		case "$choice" in
			1) run_and_pause bash "${TESTER_DIR}/scripts/smart_run.sh" "$ROOT_DIR" "${EXTRA_ARGS[@]}" ;;
			2) run_and_pause run_runner --profile quick ;;
			3) run_and_pause run_runner ;;
			4) run_and_pause run_runner --profile strict ;;
			5) run_and_pause bash "${TESTER_DIR}/scripts/diagnose.sh" "$ROOT_DIR" ;;
			6) run_and_pause bash "${TESTER_DIR}/scripts/rescue_test.sh" "$ROOT_DIR" "${EXTRA_ARGS[@]}" ;;
			7) run_and_pause run_leaks ;;
			8) function_tools ;;
			9) run_and_pause generate_html ;;
			h|H) run_and_pause run_runner --help ;;
			r|R) change_root ;;
			0) printf "Bye.\n"; exit 0 ;;
			*) printf "Invalid choice.\n"; pause ;;
		esac
	done
}

main_loop
