# Libft Tester

![CI](https://github.com/NeddyKun01/Libft-Tester/actions/workflows/ci.yml/badge.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B17-00599C)
![Project](https://img.shields.io/badge/project-42%20libft-111111)
![Output](https://img.shields.io/badge/output-OK%20%7C%20MOK%20%7C%20NOK%20%7C%20MNOK-informational)
![License](https://img.shields.io/badge/license-MIT-green)

A fast, strict, and beginner-friendly tester for the 42 `libft` project.

The main idea is simple:

```sh
make
```

`make` opens an interactive terminal menu. Advanced users and CI can use the
compiled runner directly:

```sh
./libft_tester --help
```

## Quick Start

Clone the tester next to your Libft repository:

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

If the tester lives inside the Libft repository:

```text
libft/
├── Makefile
├── libft.h
├── ft_*.c
└── tester/
```

Run:

```sh
cd libft/tester
make
```

## Interactive Menu

The menu is designed for normal day-to-day use:

```text
============================================================
 Libft Tester
============================================================
 root:     ../libft
 Makefile: OK
 libft.h:  OK
 libft.a:  OK
 runner:   OK
============================================================

Recommended
  1) Smart test
  2) Quick test
  3) Full test
  4) Strict test

Debugging
  5) Diagnose project
  6) Rescue test
  7) Leak check
  8) Explain or hint a function

Reports and help
  9) Generate HTML report
  h) Advanced CLI help
  r) Change ROOT_DIR
  0) Exit
```

Use the menu if you want the tester to guide you.

Use `./libft_tester` if you already know exactly what you want to run.

## Advanced CLI

Build the runner:

```sh
make ROOT_DIR=../libft build
```

Then use the binary directly:

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
./libft_tester --help
```

## Minimal Makefile Commands

The Makefile intentionally stays small:

| Command | Purpose |
| --- | --- |
| `make ROOT_DIR=../libft` | Open the interactive menu. |
| `make ROOT_DIR=../libft build` | Build `./libft_tester`. |
| `make self-test` | Test the tester's own diagnose/rescue behavior. |
| `make clean` | Remove tester build files and reports. |
| `make fclean` | Same as `clean`. |
| `make re ROOT_DIR=../libft` | Rebuild the runner. |

Everything else is either in the menu or in `./libft_tester`.

## Smart Test

The menu's smart test is the safest default.

It tries the normal tester first. If the Libft cannot build, link, or include
its own header correctly, it automatically runs diagnostics, tries rescue mode
when `libft.a` exists, and prints a final health summary.

Example:

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

This means the project still needs fixes, but the tester was able to test the
valid functions that already exist in `libft.a`.

## Statuses

| Status | Meaning |
| --- | --- |
| `OK` | A normal test passed. |
| `MOK` | A malloc-failure test passed. |
| `NOK` | A normal test failed. |
| `MNOK` | A malloc-failure test failed. |
| `SEGV` | The function crashed with a segmentation fault. |
| `BUS` | The function crashed with a bus error. |
| `ABRT` | The function aborted. |
| `FPE` | The function crashed with an arithmetic error. |
| `TIMEOUT` | The function took too long and was stopped. |

## Tested Suites

| Suite | Functions |
| --- | --- |
| `ctype` | `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`, `ft_toupper`, `ft_tolower` |
| `memory` | `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`, `ft_calloc` |
| `atoi` | `ft_atoi` |
| `strings` | `ft_strlen`, `ft_strchr`, `ft_strrchr`, `ft_strncmp`, `ft_strnstr`, `ft_strlcpy`, `ft_strlcat`, `ft_strdup` |
| `string_utils` | `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_itoa`, `ft_strmapi`, `ft_striteri` |
| `output` | `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd` |
| `lists` | `ft_lstnew`, `ft_lstadd_front`, `ft_lstsize`, `ft_lstlast`, `ft_lstadd_back`, `ft_lstdelone`, `ft_lstclear`, `ft_lstiter`, `ft_lstmap` |

## Reports

From the menu, choose:

```text
9) Generate HTML report
```

From the CLI:

```sh
./libft_tester --json --no-color > libft-test-report.json
./libft_tester --html --no-color > libft-test-report.html
```

## GitHub Actions

This repository includes a workflow designed to test an external Libft
repository.

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

## Requirements

| Tool | Why |
| --- | --- |
| `make` | Opens the menu and builds the runner. |
| `c++` | Compiles the C++17 tester. |
| `ar`, `nm` | Used by Libft builds and diagnostics. |
| `bash` | Runs the menu and diagnostic helpers. |
| `valgrind` | Optional, used by the leak-check menu option. |

## License

This project is released under the [MIT License](LICENSE).
