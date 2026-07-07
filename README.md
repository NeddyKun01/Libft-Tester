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
- can stop early with `--fail-fast`;
- explains function coverage with `--explain`;
- lists documented coverage with `--coverage`.

## Commands

```sh
make ROOT_DIR=../libft
make ROOT_DIR=../libft run
make ROOT_DIR=../libft ci
make ROOT_DIR=../libft leaks
```

Filter by function or suite:

```sh
make ROOT_DIR=../libft ARGS="--only ft_split"
make ROOT_DIR=../libft ARGS="--suite memory --verbose"
make ROOT_DIR=../libft ARGS="--fail-fast"
```

Explain one function:

```sh
make ROOT_DIR=../libft explain FUNC=ft_lstmap
```

## CLI

```sh
./libft_tester --help
./libft_tester --version
./libft_tester --list
./libft_tester --coverage
./libft_tester --explain ft_lstmap
./libft_tester --only ft_split
./libft_tester --suite memory
./libft_tester --timeout 5000
./libft_tester --fail-fast
./libft_tester --verbose
./libft_tester --quiet
./libft_tester --json
./libft_tester --no-color
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

- [Coverage table](docs/COVERAGE.md): tested cases per function.
- [Changelog](CHANGELOG.md): project history and pending changes.
- [Contributing guide](CONTRIBUTING.md): how to add tests and coverage.
- [GitHub Actions](.github/workflows/ci.yml): workflow ready to test an external Libft repository.

## GitHub Actions

The included workflow is designed for this tester to live in its own repository
while testing an external Libft repository.

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
├── malloc_fail.hpp
├── test_modules.hpp
└── tester.hpp

src/
├── *_tests.cpp
├── main.cpp
└── malloc_fail.cpp
```

## Note

This tester avoids testing undefined C behavior when it is not required by the
subject. For example, it does not test calls such as `ft_strlen(NULL)`.
