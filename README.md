# Libft Tester

![CI](https://github.com/NeddyKun01/Libft-Tester/actions/workflows/ci.yml/badge.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B17-00599C)
![Project](https://img.shields.io/badge/project-libft-111111)
![Output](https://img.shields.io/badge/output-OK%20%7C%20MOK%20%7C%20NOK%20%7C%20MNOK-informational)
![License](https://img.shields.io/badge/license-MIT-green)

Libft Tester is an interactive and CI-friendly tester for `libft` projects.

It is useful for students, reviewers, maintainers, and anyone working on a
small C standard-library-style project that exposes the usual `ft_*` functions.
It was built with the 42 `libft` subject in mind, but the workflow is meant to
be understandable even if you are not from 42.

The main command is intentionally simple:

```sh
make
```

That opens an interactive menu. If you prefer direct commands, build the
standalone driver and use `./libft_tester`.

## What It Checks

The tester covers:

- character checks such as `ft_isalpha`, `ft_isdigit`, `ft_toupper`;
- memory functions such as `ft_memcpy`, `ft_memmove`, `ft_calloc`;
- string functions such as `ft_strlen`, `ft_strncmp`, `ft_strnstr`;
- allocation helpers such as `ft_split`, `ft_itoa`, `ft_strtrim`;
- output helpers such as `ft_putnbr_fd`;
- linked-list helpers such as `ft_lstmap` and `ft_lstclear`;
- malloc-failure behavior for allocating functions;
- crashes, timeouts, and common build/setup problems.

## Requirements

You need:

| Tool | Why |
| --- | --- |
| `make` | Opens the menu and builds the tester. |
| `c++` | Compiles the C++17 driver and internal test suite. |
| `cc` | Builds the target C library. |
| `ar`, `nm` | Used by the target build and diagnostics. |
| `valgrind` | Optional, only needed for leak checks. |

You can check your local setup with:

```sh
./libft_tester --doctor
```

Doctor mode prints required fixes and a recommended next action when something
is missing.

Your target project should contain at least:

```text
libft/
├── Makefile
├── libft.h
└── ft_*.c
```

The target `Makefile` should build a library named `libft.a` with its default
`make` target.

## Why No Shell Scripts?

The tester is intentionally driven by C++ instead of shell helper scripts.
That makes it friendlier on school, shared, or restricted machines where script
execution permissions can be annoying.

`./libft_tester` is the standalone driver. It opens the menu, runs diagnose,
handles rescue mode, performs self-tests, and builds the internal test suite
only when real function checks are needed.

## Quick Start

Clone this tester next to your target project:

```text
projects/
├── libft/
└── Libft-Tester/
```

Run:

```sh
cd Libft-Tester
make ROOT_DIR=../libft
```

If the tester is inside your `libft` repository:

```text
libft/
├── Makefile
├── libft.h
├── ft_*.c
└── tester/
```

Run:

```sh
cd tester
make
```

If your target project is somewhere else, pass an absolute path:

```sh
make ROOT_DIR=/absolute/path/to/libft
```

## First Run Recommendation

When the menu opens, start with:

```text
1) Smart test
```

Smart test tries the normal tester first. If the target project cannot build,
link, or include its own header correctly, it automatically runs diagnostics,
tries rescue mode when `libft.a` exists, and prints a final health summary.

If everything is ready, try:

```text
3) Full test
4) Strict test
```

If something looks broken, try:

```text
5) Diagnose project
6) Rescue test
```

## Interactive Menu

The menu is designed for day-to-day use:

```text
============================================================
 Libft Tester (v1.3.1)
============================================================
 root:      ../libft
 Makefile:  OK
 libft.h:   OK
 libft.a:   OK
 suite:     OK
------------------------------------------------------------
 Recommended: Full test
 Project shape looks ready for the full suite.
============================================================

Recommended
  1) Smart test      best default, auto fallback
  2) Quick test      fast feedback
  3) Full test       normal suite
  4) Strict test     stronger validation

Debugging
  5) Diagnose project
  6) Rescue test
  7) Leak check
  8) Explain or hint a function
  d) Doctor environment

Reports and help
  9) Generate HTML report
  h) Advanced CLI help
  r) Change ROOT_DIR
  0) Exit
```

The menu uses colors when supported and respects `NO_COLOR=1`.

## Understanding Results

| Status | Meaning |
| --- | --- |
| `OK` | A normal behavior check passed. |
| `MOK` | A malloc-failure expectation passed. |
| `NOK` | A normal behavior check failed. |
| `MNOK` | A malloc-failure expectation failed. |
| `SEGV` | The tested code caused a segmentation fault. |
| `BUS` | The tested code caused a bus error. |
| `ABRT` | The tested code aborted. |
| `FPE` | The tested code caused an arithmetic error. |
| `TIMEOUT` | The suite took too long and was stopped. |

Example summary:

```text
Summary
  OKx3004 MOKx61
  checks: 3065 | failures: 0 | verdict: PASS
  pass rate: 100%
```

Example health summary when the project still needs fixes:

```text
Final Health Summary
------------------------------------------------------------
Status:       NEEDS FIXES
Normal:       FAILED
Diagnose:     PROBLEMS FOUND
Rescue:       OK

Problem counters:
- structure/build errors: 1
- missing source files: 12
- missing header declarations: 0
- missing library symbols: 12

Rescue counters:
- real symbols found: 31
- missing symbols skipped: 12
- failed tested functions: 0

Next action:
Fix the diagnose problems first, then run make again.
```

## Advanced CLI

Build the tester driver:

```sh
make ROOT_DIR=../libft build
```

Then run direct commands. The driver builds the internal suite automatically
when a command needs to execute tests against your `libft.a`:

```sh
./libft_tester --summary-only
./libft_tester --profile quick
./libft_tester --profile strict
./libft_tester --profile brutal --seed 42
./libft_tester --only ft_split --verbose
./libft_tester --suite memory
./libft_tester --repeat 10 --seed 42
./libft_tester --fail-fast
./libft_tester --json --no-color
./libft_tester --html --no-color
./libft_tester --explain ft_lstmap
./libft_tester --hint ft_split
./libft_tester --coverage
./libft_tester --doctor
./libft_tester --help
```

Use the menu when you want guidance. Use the CLI when you want automation,
filters, reports, or reproducible commands.

## Minimal Makefile Commands

The Makefile is intentionally small:

| Command | Purpose |
| --- | --- |
| `make ROOT_DIR=../libft` | Open the interactive menu. |
| `make ROOT_DIR=../libft build` | Build the standalone `./libft_tester` driver. |
| `make self-test` | Validate the tester's own fallback behavior. |
| `make clean` | Remove tester build files and reports. |
| `make fclean` | Same as `clean`. |
| `make re ROOT_DIR=../libft` | Rebuild the driver. |

Everything else is available in the menu or through `./libft_tester`.

## Reports

From the menu:

```text
9) Generate HTML report
```

From the CLI:

```sh
./libft_tester --json --no-color > libft-test-report.json
./libft_tester --html --no-color > libft-test-report.html
```

## GitHub Actions

This repository includes a workflow for testing another repository.

Set this repository variable:

```text
LIBFT_REPOSITORY=owner/repository
```

Example:

```text
LIBFT_REPOSITORY=NeddyKun01/Libft
```

The workflow builds the tester, runs the JSON suite, generates an HTML report,
checks Valgrind leaks, validates coverage metadata, and runs the tester
self-test.

## Documentation

| Document | What it explains |
| --- | --- |
| [Usage guide](docs/USAGE.md) | Menu, CLI usage, reports, and CI setup. |
| [Diagnose mode](docs/DIAGNOSE.md) | Makefile/header/archive diagnostics. |
| [Coverage table](docs/COVERAGE.md) | Tested cases per function. |
| [Troubleshooting](docs/TROUBLESHOOTING.md) | Common setup and failure fixes. |
| [Contributing tests](docs/CONTRIBUTING_TESTS.md) | How to add reliable tests. |
| [Changelog](CHANGELOG.md) | Project history and releases. |
| [Contributing guide](CONTRIBUTING.md) | How to contribute to the tester. |

## Contributing

Contributions are welcome. If you want to add tests, fix docs, improve the UI,
or report a bug, start with:

- [Contributing guide](CONTRIBUTING.md)
- [Contributing tests](docs/CONTRIBUTING_TESTS.md)
- [Issue templates](.github/ISSUE_TEMPLATE)

## License

This project is released under the [MIT License](LICENSE).
