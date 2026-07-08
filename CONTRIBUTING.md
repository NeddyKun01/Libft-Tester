# Contributing

Thank you for helping improve Libft Tester.

The project should stay useful, predictable, and easy to understand for anyone
testing a `libft`-style C library: students, reviewers, maintainers, and people
working on similar C learning projects.

## Before You Start

Keep each contribution focused on one clear goal:

- bug fix;
- new test coverage;
- documentation improvement;
- UI/output improvement;
- CI or tooling improvement.

If the change is large or changes expected behavior, please open an issue first
so the direction can be discussed.

## Development Setup

Use a local target library while developing the tester:

```sh
make ROOT_DIR=/path/to/libft
```

For direct CLI checks:

```sh
make ROOT_DIR=/path/to/libft build
./libft_tester --summary-only --seed 42
./libft_tester --only ft_split --verbose
./libft_tester --coverage
```

Validate the tester's own fallback behavior:

```sh
make self-test
```

If your change touches allocation behavior, also run a focused Valgrind check:

```sh
LIBFT_TESTER_NO_FORK=1 valgrind --leak-check=full \
  --show-leak-kinds=all --track-origins=yes \
  --errors-for-leak-kinds=all --error-exitcode=42 \
  ./libft_tester --only ft_split --no-color
```

## Adding Or Changing Tests

For the detailed test-writing guide, see
[`docs/CONTRIBUTING_TESTS.md`](docs/CONTRIBUTING_TESTS.md).

Tests live in `src/*_tests.cpp`, grouped by suite:

| File | Suite |
| --- | --- |
| `src/ctype_tests.cpp` | `ctype` |
| `src/memory_tests.cpp` | `memory` |
| `src/atoi_tests.cpp` | `atoi` |
| `src/string_tests.cpp` | `strings` |
| `src/string_utils_tests.cpp` | `string_utils` |
| `src/output_tests.cpp` | `output` |
| `src/list_tests.cpp` | `lists` |

Use the helpers from `include/tester.hpp`:

| Helper | Use |
| --- | --- |
| `tester::expect` | Boolean checks. |
| `tester::expect_eq` | Value comparisons. |
| `tester::expect_mem` | Byte-by-byte memory checks. |
| `tester::expect_malloc` | Allocation expectations. |

The first word of each label must be the function name. The reporter groups
checks by that prefix:

```cpp
tester::expect_eq(report, "ft_strlen empty", 0, ft_strlen(""));
tester::expect_malloc(report, "ft_strdup malloc failure", copy, false);
```

## Test Quality Guidelines

Prefer tests that prove different behavior:

- empty input;
- boundary sizes;
- truncation;
- first and last match;
- missing match;
- zero-length operations;
- unsigned byte behavior;
- malloc failure and cleanup paths.

Avoid adding many values that prove the same case. More checks are useful only
when they increase behavioral coverage.

## Malloc Failure Tests

Use `malloc_fail_on()` and `malloc_fail_off()` when testing allocation failures.

Always turn the failure hook off immediately after the checked call:

```cpp
malloc_fail_on(1);
result = ft_strdup("libft");
malloc_fail_off();
tester::expect_malloc(report, "ft_strdup malloc failure", result, false);
```

Expected allocation behavior should appear as `MOK` or `MNOK`.

## Undefined Behavior

Do not add tests that require undefined C behavior. The default tester should
focus on behavior required by the project specification and the relevant C/POSIX
manual pages.

Examples normally avoided:

- `ft_strlen(NULL)`;
- overlapping `ft_memcpy`;
- requiring a specific result from invalid file descriptors.

## Updating Coverage Documentation

When a test adds meaningful coverage, update:

- `src/coverage.cpp`;
- `docs/COVERAGE.md`.

You can regenerate the Markdown table with:

```sh
make ROOT_DIR=/path/to/libft build
./libft_tester --coverage-md > docs/COVERAGE.md
```

## Updating Hints

Hints live in:

```text
src/hints.cpp
```

A good hint is short, specific, and explains the most likely mistake without
pretending to know the exact bug.

## Documentation Style

Please keep documentation:

- written in clear English;
- friendly to first-time users;
- explicit about paths and commands;
- not limited to school-specific vocabulary;
- consistent with the current menu-first workflow.

## Pull Request Checklist

Before opening a pull request, run:

```sh
make ROOT_DIR=/path/to/libft build
./libft_tester --summary-only --seed 42
make self-test
```

If relevant, also run:

```sh
./libft_tester --json --no-color --only ft_strlen --seed 42 > libft-test-report.json
./libft_tester --html --no-color --only ft_strlen --seed 42 > libft-test-report.html
./libft_tester --coverage
```

If the change touches allocation behavior:

```sh
LIBFT_TESTER_NO_FORK=1 valgrind --leak-check=full \
  --show-leak-kinds=all --track-origins=yes \
  --errors-for-leak-kinds=all --error-exitcode=42 \
  ./libft_tester --only ft_split --no-color
```
