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

if [ -z "${NO_COLOR:-}" ] && [ "${TERM:-}" != "dumb" ]; then
	BOLD="$(printf '\033[1m')"
	DIM="$(printf '\033[2m')"
	RED="$(printf '\033[31m')"
	GREEN="$(printf '\033[32m')"
	YELLOW="$(printf '\033[33m')"
	CYAN="$(printf '\033[36m')"
	RESET="$(printf '\033[0m')"
else
	BOLD=""
	DIM=""
	RED=""
	GREEN=""
	YELLOW=""
	CYAN=""
	RESET=""
fi

paint()
{
	printf "%s%s%s" "$1" "$2" "$RESET"
}

ok()
{
	paint "$GREEN" "OK"
}

missing()
{
	paint "$RED" "missing"
}

status_file()
{
	if [ -f "$1" ]; then
		ok
	else
		missing
	fi
}

tester_version()
{
	local version

	if [ -x "$RUNNER" ]; then
		version="$("$RUNNER" --version 2>/dev/null | sed -n 's/^libft_tester //p')"
	else
		version="$(sed -n 's/.*g_version = "\([^"]*\)".*/\1/p' \
			"${TESTER_DIR}/src/main.cpp")"
	fi
	printf "%s" "${version:-unknown}"
}

recommended_action()
{
	if [ ! -d "$ROOT_DIR" ]; then
		printf "Change ROOT_DIR"
	elif [ ! -f "${ROOT_DIR}/Makefile" ] || [ ! -f "${ROOT_DIR}/libft.h" ]; then
		printf "Diagnose project"
	elif [ ! -f "${ROOT_DIR}/libft.a" ]; then
		printf "Smart test"
	elif [ ! -x "$RUNNER" ]; then
		printf "Quick test"
	else
		printf "Full test"
	fi
}

recommendation_hint()
{
	case "$(recommended_action)" in
		"Change ROOT_DIR")
			printf "The target folder does not exist."
			;;
		"Diagnose project")
			printf "Project files are missing or incomplete."
			;;
		"Smart test")
			printf "Build/check the project and fall back if needed."
			;;
		"Quick test")
			printf "The runner is not built yet; start with fast feedback."
			;;
		*)
			printf "Project shape looks ready for the full suite."
			;;
	esac
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
	local action

	action="$(recommended_action)"
	clear 2>/dev/null || true
	printf "\n"
	printf "%s\n" "============================================================"
	printf " %sLibft Tester%s %s(v%s)%s\n" \
		"$BOLD" "$RESET" "$DIM" "$(tester_version)" "$RESET"
	printf "%s\n" "============================================================"
	printf " %-10s %s\n" "root:" "$ROOT_DIR"
	printf " %-10s %s\n" "Makefile:" "$(status_file "${ROOT_DIR}/Makefile")"
	printf " %-10s %s\n" "libft.h:" "$(status_file "${ROOT_DIR}/libft.h")"
	printf " %-10s %s\n" "libft.a:" "$(status_file "${ROOT_DIR}/libft.a")"
	printf " %-10s %s\n" "runner:" "$(status_file "$RUNNER")"
	printf "%s\n" "------------------------------------------------------------"
	printf " %sRecommended:%s %s%s%s\n" \
		"$BOLD" "$RESET" "$CYAN" "$action" "$RESET"
	printf " %s%s%s\n" "$DIM" "$(recommendation_hint)" "$RESET"
	printf "%s\n" "============================================================"
}

print_menu()
{
	printf "\n"
	printf "%sRecommended%s\n" "$BOLD" "$RESET"
	printf "  %s1%s) Smart test      %sbest default, auto fallback%s\n" \
		"$CYAN" "$RESET" "$DIM" "$RESET"
	printf "  %s2%s) Quick test      %sfast feedback%s\n" \
		"$CYAN" "$RESET" "$DIM" "$RESET"
	printf "  %s3%s) Full test       %snormal suite%s\n" \
		"$CYAN" "$RESET" "$DIM" "$RESET"
	printf "  %s4%s) Strict test     %sstronger validation%s\n" \
		"$CYAN" "$RESET" "$DIM" "$RESET"
	printf "\n"
	printf "%sDebugging%s\n" "$BOLD" "$RESET"
	printf "  %s5%s) Diagnose project\n" "$YELLOW" "$RESET"
	printf "  %s6%s) Rescue test\n" "$YELLOW" "$RESET"
	printf "  %s7%s) Leak check\n" "$YELLOW" "$RESET"
	printf "  %s8%s) Explain or hint a function\n" "$YELLOW" "$RESET"
	printf "\n"
	printf "%sReports and help%s\n" "$BOLD" "$RESET"
	printf "  %s9%s) Generate HTML report\n" "$GREEN" "$RESET"
	printf "  %sh%s) Advanced CLI help\n" "$GREEN" "$RESET"
	printf "  %sr%s) Change ROOT_DIR\n" "$GREEN" "$RESET"
	printf "  %s0%s) Exit\n" "$GREEN" "$RESET"
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
			0) printf "%s\n" "Bye."; exit 0 ;;
			*) printf "Invalid choice.\n"; pause ;;
		esac
	done
}

main_loop
