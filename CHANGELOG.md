# Changelog

All notable changes to this project will be documented in this file.

## Unreleased

## 1.7.0 - 2026-07-15

- Added `--review` mode with a compact reviewer-friendly verdict, score,
  failed-function list, crash list, malloc-failure list, and rerun commands.
- Expanded the HTML report with filter counters for all, passed, failed,
  malloc-related, and crash-related functions.
- Added per-function copyable rerun commands and a `Likely Fixes` section to
  the HTML report.
- Added optional `.libft-tester.json` config support for `root`, `profile`,
  `seed`, and `no_color`, while keeping CLI arguments as the final override.
- Strengthened self-tests for review output, config-file behavior, HTML report
  filters, filter metadata, and likely-fix sections.
- Updated public documentation for review mode, config files, and the richer
  HTML report workflow.

## 1.6.0 - 2026-07-13

- Added a clearer HTML report with a `Score Guide` explaining that every
  `X/Y` score means `passed/total`.
- Added HTML report filters for all, failed, and passed functions.
- Replaced ambiguous HTML summary ratios such as `OK/MOK` with explicit
  `Passed` and `Failed` metrics.
- Renamed terminal and HTML function score headers to `OK/Total`.
- Added output contract self-tests for terminal section order, score labels,
  HTML filters, filter metadata, and score-guide content.
- Updated README, usage, and contribution docs with the score convention and
  report filtering behavior.
- Added visual README previews for the interactive menu and HTML report.

## 1.5.0 - 2026-07-13

- Polished the interactive menu with a compact health line, explicit
  recommended key, and matching CLI command for the current target.
- Added `Debug Focus` after failing runs, including copy/paste-ready commands
  for the first failed functions, the active `--root`, and the run seed.
- Made `--coverage`, `--coverage-md`, `--explain`, and `--hint` available
  directly from the driver without requiring the target Libft to build.
- Isolated internal build directories by target `ROOT_DIR` to avoid collisions
  between different targets or concurrent self-test/full-test runs.
- Updated README, usage, troubleshooting, and contribution docs around the
  `./libft_tester`-first workflow.
- Improved `--help` with recommended modern examples.

## 1.4.0 - 2026-07-12

- Rescue failures now print inline terminal details instead of sending users to
  per-function `.log` files.
- Suite crashes are now shown as separate `Runner Issues` instead of being
  mixed into the function results as confusing pseudo-function failures.
- High-risk `string_utils` and `output` checks now run with per-function/case
  isolation so crashes point to the exact Libft function and scenario.
- Reorganized tests into `tester/tests/` with one `.cpp` file per Libft
  function, leaving `tester/src/` focused on the tester runtime.
- Moved tester source code under `tester/` so the repository root stays focused
  on project docs, metadata, and the main Makefile.

## 1.3.1 - 2026-07-09

- Split the C++ driver into smaller modules for process helpers, diagnose,
  rescue/smart run, menu, self-test, and doctor checks.
- Added `./libft_tester --doctor` to check required tools and target project
  shape without running the full suite.
- Added self-test coverage for doctor mode diagnostics and warning-only
  project states.
- Improved doctor output with per-problem `Fix:` hints and a final
  `Next action:` recommendation.

## 1.3.0 - 2026-07-08

- Replaced the shell-script entrypoints with a standalone C++ driver for menu,
  diagnose, smart run, rescue mode, and self-test.
- Split the executable architecture so `./libft_tester` no longer links against
  the target Libft directly; real function tests run through an internal
  `tester/build/libft_suite` binary.
- Removed the `scripts/` helpers after moving their behavior into the C++
  driver.

## 1.2.0 - 2026-07-08

- Simplified the Makefile around an interactive `make` menu and moved advanced
  usage toward the `./libft_tester` CLI.
- Polished the interactive menu with colored status labels, version display,
  contextual recommendations, and a clearer health summary.
- Reworked public documentation, contribution docs, security policy, and issue
  templates for clearer English onboarding beyond school-specific audiences.

## 1.1.1 - 2026-07-08

- Reworked the README into a clearer public usage guide with quick-start,
  command tables, output examples, diagnose/rescue explanations, and CI notes.
- Added a smart default `make` flow that falls back to diagnose/rescue and
  prints a final health summary when the normal tester cannot finish.
- Updated GitHub Actions to newer checkout/artifact actions to remove Node.js
  deprecation warnings.

## 1.1.0 - 2026-07-07

- Added `make diagnose` for Makefile/header/source/archive diagnostics.
- Added `make rescue-test` to test only functions with real `libft.a` symbols.
- Added weak rescue stubs for missing functions during partial testing.
- Added `make self-test` with temporary broken Libft fixtures.
- Added a GitHub Actions self-test job for diagnose/rescue fixtures.
- Added known-good reference files in `tester/templates/libft.h` and
  `tester/templates/Makefile`.
- Added `docs/DIAGNOSE.md`.

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
