# Changelog

All notable changes to this project will be documented in this file.

## Unreleased

- Improved `ROOT_DIR` validation with clear setup errors.
- Moved coverage metadata out of `main.cpp` into dedicated coverage files.
- Added documented coverage commands with `--coverage` and `--explain`.
- Added GitHub Actions support for testing an external Libft repository.
- Added `docs/COVERAGE.md` with per-function coverage documentation.

## 1.0.0-dev

- Added fork-isolated suite execution.
- Added compact per-function terminal output.
- Added `OK`, `NOK`, `MOK`, and `MNOK` statuses.
- Added `malloc` failure simulation.
- Added JSON output for scripts and CI.
- Added `valgrind` leak-checking target.
