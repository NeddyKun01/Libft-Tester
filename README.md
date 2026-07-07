# Libft Tester

![CI](https://github.com/NeddyKun01/Libft-Tester/actions/workflows/ci.yml/badge.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B17-00599C)
![Project](https://img.shields.io/badge/project-42%20libft-111111)
![Output](https://img.shields.io/badge/output-OK%20%7C%20MOK%20%7C%20NOK%20%7C%20MNOK-informational)
![License](https://img.shields.io/badge/license-MIT-green)

A C++ tester for 42 `libft` projects, designed to be fast, readable, and useful
during development, debugging, and peer evaluation.

## Quick Start

With the tester next to your Libft repository:

```text
projects/
├── libft/
└── Libft-Tester/
```

```sh
cd Libft-Tester
make ROOT_DIR=../libft
```

With the tester inside your Libft repository:

```text
libft/
├── Makefile
├── libft.h
├── ft_*.c
└── tester/
```

```sh
cd libft/tester
make
```

## What It Does

- tests every function from the current `libft` subject;
- prints compact per-function results;
- uses `OK`, `NOK`, `MOK`, and `MNOK`;
- isolates suites with `fork`;
- detects `SEGV`, `BUS`, `ABRT`, `FPE`, and `TIMEOUT`;
- simulates `malloc` failures;
- provides a `valgrind` leak-checking mode;
- exports JSON for scripts and CI;
- exports standalone HTML reports;
- can stop early with `--fail-fast`;
- provides run profiles with `--profile`;
- has a stronger `--strict` shortcut;
- can repeat runs with `--repeat`;
- can reproduce pseudo-random tests with `--seed`;
- can print a compact `--summary-only` view;
- shows beginner-friendly debugging hints with `--hint`;
- explains function coverage with `--explain`;
- lists documented coverage with `--coverage`.

## Commands

```sh
make ROOT_DIR=../libft
make ROOT_DIR=../libft run
make ROOT_DIR=../libft ci
make ROOT_DIR=../libft report
make ROOT_DIR=../libft report-html
make ROOT_DIR=../libft summary
make ROOT_DIR=../libft strict
make ROOT_DIR=../libft leaks
```

Filter by function or suite:

```sh
make ROOT_DIR=../libft ARGS="--only ft_split"
make ROOT_DIR=../libft ARGS="--suite memory --verbose"
make ROOT_DIR=../libft ARGS="--fail-fast"
make ROOT_DIR=../libft ARGS="--repeat 10 --seed 42"
make ROOT_DIR=../libft ARGS="--profile brutal --summary-only"
```

Explain or debug one function:

```sh
make ROOT_DIR=../libft explain FUNC=ft_lstmap
make ROOT_DIR=../libft hint FUNC=ft_split
make ROOT_DIR=../libft coverage-docs
```

## CLI

```sh
./libft_tester --help
./libft_tester --version
./libft_tester --list
./libft_tester --profiles
./libft_tester --coverage
./libft_tester --explain ft_lstmap
./libft_tester --hint ft_split
./libft_tester --only ft_split
./libft_tester --suite memory
./libft_tester --profile quick
./libft_tester --profile strict
./libft_tester --profile brutal
./libft_tester --timeout 5000
./libft_tester --repeat 10
./libft_tester --seed 42
./libft_tester --strict
./libft_tester --fail-fast
./libft_tester --summary-only
./libft_tester --verbose
./libft_tester --quiet
./libft_tester --json
./libft_tester --html
./libft_tester --no-color
```

## Profiles

Profiles are shortcuts for common testing moods:

| Profile | Repeats | Timeout | Fail-fast | Use case |
| --- | --- | --- | --- | --- |
| `quick` | 1 | 1500 ms | yes | fast feedback while coding |
| `normal` | 1 | 3000 ms | no | default balanced run |
| `strict` | 10 | 6000 ms | no | stronger local validation |
| `brutal` | 25 | 9000 ms | no | heavy run before push/release |

```sh
./libft_tester --profile quick --only ft_split
./libft_tester --strict --summary-only
./libft_tester --profile brutal --seed 42
```

Manual flags still work with profiles. For example, this uses the `brutal`
timeout/fail-fast behavior but overrides the repeat count:

```sh
./libft_tester --profile brutal --repeat 3
```

## Statuses

| Status | Meaning |
| --- | --- |
| `OK` | Normal test passed. |
| `MOK` | `malloc`-related expectation passed. |
| `NOK` | Normal test failed. |
| `MNOK` | `malloc`-related expectation failed. |
| `SEGV` | The suite crashed with a segmentation fault. |
| `BUS` | The suite crashed with a bus error. |
| `ABRT` | The suite aborted. |
| `FPE` | The suite crashed with an arithmetic error. |
| `TIMEOUT` | The suite took too long to finish. |

## Suites

| Suite | Functions |
| --- | --- |
| `ctype` | `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`, `ft_toupper`, `ft_tolower` |
| `memory` | `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`, `ft_calloc` |
| `atoi` | `ft_atoi` |
| `strings` | `ft_strlen`, `ft_strchr`, `ft_strrchr`, `ft_strncmp`, `ft_strnstr`, `ft_strlcpy`, `ft_strlcat`, `ft_strdup` |
| `string_utils` | `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_itoa`, `ft_strmapi`, `ft_striteri` |
| `output` | `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd` |
| `lists` | `ft_lstnew`, `ft_lstadd_front`, `ft_lstsize`, `ft_lstlast`, `ft_lstadd_back`, `ft_lstdelone`, `ft_lstclear`, `ft_lstiter`, `ft_lstmap` |

## Documentation

- [Usage guide](docs/USAGE.md): commands, profiles, reports, and CI setup.
- [Coverage table](docs/COVERAGE.md): tested cases per function.
- [Troubleshooting](docs/TROUBLESHOOTING.md): common setup and failure fixes.
- [Contributing tests](docs/CONTRIBUTING_TESTS.md): how to add reliable tests.
- [Changelog](CHANGELOG.md): project history and pending changes.
- [Contributing guide](CONTRIBUTING.md): how to add tests and coverage.
- [GitHub Actions](.github/workflows/ci.yml): workflow ready to test an external Libft repository.

## GitHub Actions

The included workflow is designed for this tester to live in its own repository
while testing an external Libft repository.

It runs separate jobs for the main JSON suite, Valgrind leak checks, and coverage
metadata. The JSON report, standalone HTML report, Valgrind log, and coverage
report are uploaded as GitHub Actions artifacts.

You can use it in two ways:

- manually, with `workflow_dispatch`, by providing the Libft `owner/repo`;
- automatically, by setting the `LIBFT_REPOSITORY` repository variable.

Example:

```text
NeddyKun01/Libft
```

## Structure

```text
include/
├── coverage.hpp
├── hints.hpp
├── malloc_fail.hpp
├── profiles.hpp
├── test_modules.hpp
└── tester.hpp

src/
├── *_tests.cpp
├── coverage.cpp
├── hints.cpp
├── main.cpp
├── malloc_fail.cpp
└── profiles.cpp
```

## Reproducibility

When `--seed` is provided, pseudo-random tests use the same generated cases on
every run. If no seed is provided, the tester prints the generated seed in the
summary and JSON output so the run can be repeated later.

```sh
./libft_tester --repeat 20 --seed 42
./libft_tester --summary-only --repeat 20 --seed 42
```

## Reports And Hints

For scripts and CI:

```sh
./libft_tester --json --no-color > libft-test-report.json
```

For a visual artifact that can be opened in a browser:

```sh
./libft_tester --html --no-color > libft-test-report.html
```

For quick debugging advice:

```sh
./libft_tester --hint ft_split
```

When a test fails in normal terminal output or in the HTML report, the tester
also prints a small hint for the related function.

## Note

This tester avoids testing undefined C behavior when it is not required by the
subject. For example, it does not test calls such as `ft_strlen(NULL)`.
