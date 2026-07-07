# Contributing

Thanks for helping improve Libft Tester. The goal is to keep the project useful,
predictable, and easy to understand for 42 students reviewing their `libft`.

## Development Setup

Use a local Libft repository as the target:

```sh
make ROOT_DIR=/path/to/libft
make ROOT_DIR=/path/to/libft ci
make ROOT_DIR=/path/to/libft leaks ARGS="--only ft_split"
```

Before opening a pull request, run at least:

```sh
make ROOT_DIR=/path/to/libft run ARGS="--no-color"
make ROOT_DIR=/path/to/libft ci ARGS="--only ft_strlen"
make ROOT_DIR=/path/to/libft coverage
```

## Adding Tests

Tests live in `src/*_tests.cpp`, grouped by suite:

- `ctype_tests.cpp`
- `memory_tests.cpp`
- `atoi_tests.cpp`
- `string_tests.cpp`
- `string_utils_tests.cpp`
- `output_tests.cpp`
- `list_tests.cpp`

Use the existing helpers from `include/tester.hpp`:

- `tester::expect` for boolean checks.
- `tester::expect_eq` for value comparisons.
- `tester::expect_mem` for byte-by-byte memory checks.
- `tester::expect_malloc` for allocation expectations.

Keep labels consistent. The first word of the label must be the function name,
because the reporter groups checks by that prefix:

```cpp
tester::expect_eq(report, "ft_strlen empty", 0, ft_strlen(""));
tester::expect_malloc(report, "ft_strdup malloc failure", copy, false);
```

## Malloc Failure Tests

Use `malloc_fail_on()` and `malloc_fail_off()` when testing allocation failures.
Always turn the failure hook off after the checked call.

Expected allocation behavior should be reported with `MOK`/`MNOK` through
`tester::expect_malloc`.

## Undefined Behavior

Do not add tests for undefined C behavior unless they are placed behind a future
explicit strict mode. Normal mode should focus on behavior required by the 42
subject and by the relevant manual pages.

Examples normally avoided:

- `ft_strlen(NULL)`
- overlapping `ft_memcpy`
- writing through invalid file descriptors as a correctness requirement

## Coverage Documentation

When adding or changing meaningful test coverage, update both:

- `src/coverage.cpp`
- `docs/COVERAGE.md`

This keeps `--coverage`, `--explain`, and the documentation aligned.

## Style

- Prefer readable tests over clever tests.
- Keep one clear behavior per check label.
- Avoid duplicate tests that only repeat the same case with different values.
- Add malloc-failure coverage for functions that allocate.
- Keep output deterministic so CI remains stable.
