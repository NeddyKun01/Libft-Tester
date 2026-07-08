#!/usr/bin/env bash

set -u

ROOT_DIR="${1:-..}"
shift || true
EXTRA_ARGS=("$@")
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTER_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
LIBFT="${ROOT_DIR}/libft.a"
RUNNER="${TESTER_DIR}/libft_tester"
ARG_STRING="${EXTRA_ARGS[*]}"

normal_status=0
diagnose_status=0
rescue_status=0
rescue_ran=0

cleanup()
{
	rm -f "$normal_log" "$diagnose_log" "$rescue_log"
}

section()
{
	printf "\n%s\n" "$1"
	printf "%s\n" "------------------------------------------------------------"
}

value_from_log()
{
	local label="$1"
	local file="$2"

	awk -F': ' -v label="$label" '$1 == label {print $2}' "$file" \
		| tail -n 1
}

print_normal_excerpt()
{
	section "Normal Run Result"
	printf "The normal tester did not finish successfully.\n"
	printf "This can happen because of build/header/link errors or real test failures.\n"
	printf "\nFirst messages from the normal run:\n"
	sed -n '1,40p' "$normal_log" | sed 's/^/  /'
}

run_normal()
{
	{
		make -s -C "$TESTER_DIR" build ROOT_DIR="$ROOT_DIR" \
			&& "$RUNNER" "${EXTRA_ARGS[@]}"
	} >"$normal_log" 2>&1
}

run_diagnose()
{
	bash "${TESTER_DIR}/scripts/diagnose.sh" "$ROOT_DIR" \
		>"$diagnose_log" 2>&1
}

run_rescue()
{
	bash "${TESTER_DIR}/scripts/rescue_test.sh" "$ROOT_DIR" \
		"${EXTRA_ARGS[@]}" >"$rescue_log" 2>&1
}

print_final_summary()
{
	local build_errors
	local build_warnings
	local missing_sources
	local missing_headers
	local missing_symbols
	local real_symbols
	local skipped_symbols
	local failed_functions

	build_errors="$(value_from_log "structure/build errors" "$diagnose_log")"
	build_warnings="$(value_from_log "structure/build warnings" "$diagnose_log")"
	missing_sources="$(value_from_log "missing source files" "$diagnose_log")"
	missing_headers="$(value_from_log "missing header declarations" "$diagnose_log")"
	missing_symbols="$(value_from_log "missing library symbols" "$diagnose_log")"
	real_symbols="$(value_from_log "real symbols found" "$rescue_log")"
	skipped_symbols="$(value_from_log "missing symbols skipped" "$rescue_log")"
	failed_functions="$(value_from_log "failed tested functions" "$rescue_log")"

	section "Final Health Summary"
	printf "Normal tests: FAILED\n"
	printf "Diagnose: %s\n" "$([ "$diagnose_status" -eq 0 ] && printf "OK" || printf "PROBLEMS FOUND")"
	if [ "$rescue_ran" -eq 1 ]; then
		printf "Rescue test: %s\n" "$([ "$rescue_status" -eq 0 ] && printf "OK" || printf "FAILED")"
	else
		printf "Rescue test: SKIPPED because libft.a was not available\n"
	fi
	printf "\nProblem counters:\n"
	printf "%s\n" "- structure/build errors: ${build_errors:-unknown}"
	printf "%s\n" "- structure/build warnings: ${build_warnings:-unknown}"
	printf "%s\n" "- missing source files: ${missing_sources:-unknown}"
	printf "%s\n" "- missing header declarations: ${missing_headers:-unknown}"
	printf "%s\n" "- missing library symbols: ${missing_symbols:-unknown}"
	if [ "$rescue_ran" -eq 1 ]; then
		printf "\nRescue counters:\n"
		printf "%s\n" "- real symbols found: ${real_symbols:-unknown}"
		printf "%s\n" "- missing symbols skipped: ${skipped_symbols:-unknown}"
		printf "%s\n" "- failed tested functions: ${failed_functions:-unknown}"
	fi
	printf "\nRecommendation:\n"
	if [ "$diagnose_status" -ne 0 ]; then
		printf "Fix the diagnose problems first, then run make again.\n"
	elif [ "$rescue_status" -ne 0 ]; then
		printf "The structure looks usable, but some tested functions still fail.\n"
	else
		printf "The fallback tests passed, but the normal tester still failed. Check the normal run messages above.\n"
	fi
}

main()
{
	normal_log="$(mktemp)"
	diagnose_log="$(mktemp)"
	rescue_log="$(mktemp)"
	trap cleanup EXIT

	printf "\nLibft Tester Smart Run\n"
	printf "root: %s\n" "$ROOT_DIR"
	if [ "${#EXTRA_ARGS[@]}" -gt 0 ]; then
		printf "args: %s\n" "$ARG_STRING"
	fi
	if run_normal; then
		cat "$normal_log"
		section "Final Health Summary"
		printf "Normal tests: OK\n"
		printf "Diagnose: not needed\n"
		printf "Rescue test: not needed\n"
		exit 0
	fi
	normal_status=$?
	print_normal_excerpt
	section "Diagnose Fallback"
	if run_diagnose; then
		diagnose_status=0
	else
		diagnose_status=$?
	fi
	cat "$diagnose_log"
	section "Rescue Fallback"
	if [ -f "$LIBFT" ]; then
		rescue_ran=1
		if run_rescue; then
			rescue_status=0
		else
			rescue_status=$?
		fi
		cat "$rescue_log"
	else
		printf "Skipped: libft.a was not found, so there are no symbols to test.\n"
	fi
	print_final_summary
	exit 1
}

main "$@"
