# Changelog

All notable changes to this project will be documented in this file.

## Unreleased

- Nothing yet.

## 1.0.0 - 2026-07-07

- Added run profiles with `--profile quick|normal|strict|brutal`.
- Added `--strict` shortcut for stronger local validation.
- Added standalone HTML reports with `--html` and `make report-html`.
- Added debugging hints with `--hint` and automatic hints on failures.
- Added HTML report artifacts to GitHub Actions.
- Added `--repeat`, `--seed`, and `--summary-only`.
- Added reproducible pseudo-random checks for memory and string functions.
- Expanded JSON reports with metadata and individual check details.
- Added `--coverage-md` and `make coverage-docs` to generate coverage docs.
- Improved GitHub Actions with separate test, leak, and coverage jobs.
- Added CI artifacts for JSON, Valgrind, and coverage reports.
- Added `make report` to write JSON output to `libft-test-report.json`.
- Improved `ROOT_DIR` validation with clear setup errors.
- Moved coverage metadata out of `main.cpp` into dedicated coverage files.
- Added `--fail-fast` to stop after the first failing suite.
- Added `CONTRIBUTING.md` with testing and coverage guidelines.
- Added documented coverage commands with `--coverage` and `--explain`.
- Added GitHub Actions support for testing an external Libft repository.
- Added `docs/COVERAGE.md` with per-function coverage documentation.
- Added `docs/USAGE.md`, `docs/TROUBLESHOOTING.md`, and
  `docs/CONTRIBUTING_TESTS.md`.

## 1.0.0-dev

- Added fork-isolated suite execution.
- Added compact per-function terminal output.
- Added `OK`, `NOK`, `MOK`, and `MNOK` statuses.
- Added `malloc` failure simulation.
- Added JSON output for scripts and CI.
- Added `valgrind` leak-checking target.
