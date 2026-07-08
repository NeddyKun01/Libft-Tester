# Libft Tester

![CI](https://github.com/NeddyKun01/Libft-Tester/actions/workflows/ci.yml/badge.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B17-00599C)
![Project](https://img.shields.io/badge/project-42%20libft-111111)
![Output](https://img.shields.io/badge/output-OK%20%7C%20MOK%20%7C%20NOK%20%7C%20MNOK-informational)
![License](https://img.shields.io/badge/license-MIT-green)

A fast, strict, and beginner-friendly tester for the 42 `libft` project.

It is built to help during development, peer evaluation, and CI. The goal is not
only to say that something failed, but to make it obvious what failed, why it
probably failed, and what to check next.

## Quick Start

Clone the tester next to your Libft repository:

```text
projects/
├── libft/
└── Libft-Tester/
```

Run the tester:

```sh
cd Libft-Tester
make ROOT_DIR=../libft
```

`make` is a smart entrypoint. It first tries the normal tester. If the Libft
cannot build, link, or include its own header correctly, it automatically runs
diagnose mode, tries rescue mode when `libft.a` exists, and prints a final
health summary.

If your Libft is somewhere else, pass the absolute path:

```sh
make ROOT_DIR=/absolute/path/to/libft
```

For a compact pass/fail result:

```sh
make ROOT_DIR=../libft summary
```

For a stronger check before pushing:

```sh
make ROOT_DIR=../libft strict
```

## Recommended First Run

If this is the first time you test a Libft with this project, run these commands
in order:

```sh
make ROOT_DIR=../libft diagnose
make ROOT_DIR=../libft summary
make ROOT_DIR=../libft rescue-test
```

What each command tells you:

| Step | Command | Purpose |
| --- | --- | --- |
| 1 | `make diagnose` | Checks if the Libft structure, `Makefile`, `libft.h`, sources, and archive symbols look correct. |
| 2 | `make summary` | Runs the normal tester with compact output. |
| 3 | `make rescue-test` | Tests valid functions even if the user's header or Makefile is partially broken. |

## Output Example

Normal compact output looks like this:

```text
ft_strlen [7/7]
OK OK OK OK OK OK OK

ft_calloc [8/8]
OK OK OK OK MOK MOK OK OK

Summary
  OKx3004 MOKx61
  checks: 3065 | failures: 0 | verdict: PASS
```

The important idea:

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

## Commands

| Command | Use when you want to... |
| --- | --- |
| `make ROOT_DIR=../libft` | Run the smart tester with automatic diagnose/rescue fallback. |
| `make ROOT_DIR=../libft normal` | Run the old pure build-and-test flow without fallback. |
| `make ROOT_DIR=../libft summary` | See only the compact final result. |
| `make ROOT_DIR=../libft quick` | Get very fast feedback while coding. |
| `make ROOT_DIR=../libft strict` | Run stronger validation before a push. |
| `make ROOT_DIR=../libft brutal` | Run a heavier stress profile. |
| `make ROOT_DIR=../libft diagnose` | Check project structure, header declarations, and archive symbols. |
| `make ROOT_DIR=../libft rescue-test` | Test functions that exist even when other functions are missing. |
| `make ROOT_DIR=../libft leaks` | Run Valgrind leak and invalid-access checks. |
| `make ROOT_DIR=../libft report` | Save a JSON report to `libft-test-report.json`. |
| `make ROOT_DIR=../libft report-html` | Save a standalone HTML report to `libft-test-report.html`. |
| `make self-test` | Test the tester itself with temporary broken Libft fixtures. |

Useful filtered runs:

```sh
make ROOT_DIR=../libft ARGS="--only ft_split"
make ROOT_DIR=../libft ARGS="--suite memory --verbose"
make ROOT_DIR=../libft ARGS="--fail-fast"
make ROOT_DIR=../libft ARGS="--repeat 10 --seed 42"
make ROOT_DIR=../libft ARGS="--profile brutal --summary-only"
```

Useful documentation commands:

```sh
make ROOT_DIR=../libft explain FUNC=ft_lstmap
make ROOT_DIR=../libft hint FUNC=ft_split
make ROOT_DIR=../libft coverage-docs
```

## Diagnose Mode

The default `make` target already calls this automatically when the normal
tester cannot finish successfully.

`make diagnose` is the best command when the tester cannot build, when symbols
are missing, or when you are not sure if the Libft project follows the expected
subject structure.

Example:

```sh
make ROOT_DIR=../libft diagnose
```

Example output:

```text
Function          Source  Header  Symbol  Status           Suggested fix
--------          ------  ------  ------  ------           -------------
ft_strlen         yes     yes     yes     OK               -
ft_split          yes     no      yes     HEADER MISSING   add prototype to libft.h
ft_lstmap         yes     yes     no      SYMBOL MISSING   ensure source is compiled into libft.a
```

How to read it:

| Problem | Usually means |
| --- | --- |
| `SOURCE MISSING` | The `ft_*.c` file is missing from the Libft directory. |
| `HEADER MISSING` | The function prototype is missing from `libft.h`. |
| `SYMBOL MISSING` | The source may exist, but it was not compiled into `libft.a`. |
| `Makefile found: no` | `ROOT_DIR` points to the wrong folder or the Libft is incomplete. |
| `libft.h found: no` | The header is missing or named incorrectly. |

## Rescue Test Mode

The default `make` target already tries this automatically when the normal
tester fails and a `libft.a` archive exists.

`make rescue-test` exists for a very practical reason: sometimes a Libft has a
broken header, a missing prototype, or a partial archive, but many functions are
still valid and deserve to be tested.

This mode uses known-good model files from:

```text
templates/libft.h
templates/Makefile
```

It then tests the real symbols found in the user's `libft.a` and skips missing
ones instead of failing the whole run immediately.

Example:

```text
Function          Status
--------          ------
ft_strlen         OK
ft_split          SKIP missing symbol
ft_lstclear       OK

Summary
real symbols found: 42
missing symbols skipped: 1
failed tested functions: 0
```

Important: rescue mode does not hide structural problems. Use it together with
`make diagnose`. Diagnose tells you what must be fixed; rescue-test tells you
what can still be tested safely.

## Smart Run Summary

When the normal tester fails before finishing, the default `make` target prints
a final summary like this:

```text
Final Health Summary
------------------------------------------------------------
Normal tests: FAILED
Diagnose: PROBLEMS FOUND
Rescue test: OK

Problem counters:
- structure/build errors: 1
- structure/build warnings: 0
- missing source files: 12
- missing header declarations: 0
- missing library symbols: 12

Rescue counters:
- real symbols found: 31
- missing symbols skipped: 12
- failed tested functions: 0
```

This means the project still needs fixes, but the tester was able to test the
valid functions that already exist in `libft.a`.

## Profiles

Profiles are shortcuts for common testing needs:

| Profile | Repeats | Timeout | Fail-fast | Best for |
| --- | --- | --- | --- | --- |
| `quick` | 1 | 1500 ms | yes | Fast feedback while coding. |
| `normal` | 1 | 3000 ms | no | Default balanced run. |
| `strict` | 10 | 6000 ms | no | Strong local validation. |
| `brutal` | 25 | 9000 ms | no | Heavy run before release or public sharing. |

Examples:

```sh
make ROOT_DIR=../libft quick
make ROOT_DIR=../libft strict
make ROOT_DIR=../libft brutal ARGS="--seed 42"
```

Manual flags can override profile defaults:

```sh
make ROOT_DIR=../libft ARGS="--profile brutal --repeat 3"
```

## CLI

After the tester is built, you can run the executable directly:

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

JSON report:

```sh
make ROOT_DIR=../libft report
```

Direct CLI version:

```sh
./libft_tester --json --no-color > libft-test-report.json
```

HTML report:

```sh
make ROOT_DIR=../libft report-html
```

Direct CLI version:

```sh
./libft_tester --html --no-color > libft-test-report.html
```

The HTML report is standalone, so it can be opened in a browser or uploaded as a
CI artifact.

## Debugging Help

For quick advice about one function:

```sh
make ROOT_DIR=../libft hint FUNC=ft_split
```

For the documented cases covered by one function:

```sh
make ROOT_DIR=../libft explain FUNC=ft_lstmap
```

For the full coverage table:

```sh
make ROOT_DIR=../libft coverage
```

When a test fails in normal terminal output or in the HTML report, the tester
also prints a small hint for the related function.

## GitHub Actions

This repository includes a workflow designed to test an external Libft
repository.

It can run:

| Job | Purpose |
| --- | --- |
| Main tester | Runs the JSON suite. |
| Valgrind leaks | Checks leaks and invalid memory access. |
| Coverage metadata | Verifies documented coverage output. |
| Tester self-test | Validates diagnose and rescue behavior using broken fixtures. |

The workflow can be used manually with `workflow_dispatch`, or automatically by
setting the repository variable:

```text
LIBFT_REPOSITORY=NeddyKun01/Libft
```

The value must be in this format:

```text
owner/repository
```

Artifacts produced by CI may include:

| Artifact | Contains |
| --- | --- |
| JSON report | Machine-readable test result. |
| HTML report | Browser-friendly report. |
| Valgrind log | Leak and invalid-access information. |
| Coverage report | Documented coverage metadata. |

## Project Structure

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

scripts/
├── diagnose.sh
├── rescue_test.sh
└── self_test.sh

templates/
├── libft.h
└── Makefile
```

## Documentation

| Document | What it explains |
| --- | --- |
| [Usage guide](docs/USAGE.md) | Commands, profiles, reports, and CI setup. |
| [Diagnose mode](docs/DIAGNOSE.md) | Makefile/header/archive diagnostics. |
| [Coverage table](docs/COVERAGE.md) | Tested cases per function. |
| [Troubleshooting](docs/TROUBLESHOOTING.md) | Common setup and failure fixes. |
| [Contributing tests](docs/CONTRIBUTING_TESTS.md) | How to add reliable tests. |
| [Changelog](CHANGELOG.md) | Project history and releases. |
| [Contributing guide](CONTRIBUTING.md) | How to contribute to the tester. |
| [GitHub Actions](.github/workflows/ci.yml) | CI workflow for external Libft repositories. |

## Reproducibility

When `--seed` is provided, pseudo-random tests use the same generated cases on
every run.

```sh
make ROOT_DIR=../libft ARGS="--repeat 20 --seed 42"
make ROOT_DIR=../libft ARGS="--summary-only --repeat 20 --seed 42"
```

If no seed is provided, the tester prints the generated seed in the summary and
JSON output so the run can be repeated later.

## What This Tester Avoids

This tester avoids requiring undefined C behavior when it is not part of the 42
subject.

For example, it does not require calls such as:

```c
ft_strlen(NULL);
```

If a function is expected to behave like the original libc function, the tester
focuses on defined and meaningful behavior for that function.

## Requirements

You need:

| Tool | Why |
| --- | --- |
| `make` | Builds the tester and the target Libft. |
| `c++` | Compiles the C++17 tester. |
| `ar`, `nm` | Used indirectly by Libft builds and diagnose mode. |
| `bash` | Runs diagnostic scripts. |
| `valgrind` | Optional, only needed for `make leaks`. |

## License

This project is released under the [MIT License](LICENSE).
