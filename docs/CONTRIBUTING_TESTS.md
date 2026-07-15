# Contributing Tests

This guide explains how to add or change tests without making the tester harder
to maintain.

## Where Tests Live

Runtime code lives in `tester/src/`. Function tests live in `tester/tests/`.

Use one `.cpp` file per Libft function:

| File pattern | Purpose |
| --- | --- |
| `tester/tests/ft_strlen.cpp` | Tests for `ft_strlen`. |
| `tester/tests/ft_split.cpp` | Tests for `ft_split`. |
| `tester/tests/ft_lstmap.cpp` | Tests for `ft_lstmap`. |
| `tester/tests/suites.cpp` | Groups function tests into suites. |
| `tester/tests/test_helpers.hpp` | Shared helpers used only by tests. |

When adding a new function test, place the checks in
`tester/tests/ft_function.cpp` and register that function in
`tester/tests/suites.cpp`.

## Check Helpers

Use the helpers from `tester/include/tester.hpp`:

```cpp
tester::expect(report, "ft_isalpha letter", condition);
tester::expect_eq(report, "ft_strlen empty", expected, actual);
tester::expect_mem(report, "ft_memcpy bytes", expected, actual, size);
tester::expect_malloc(report, "ft_strdup malloc failure", ptr, false);
```

The first word of each label must be the function name. The reporter uses that
prefix to group results:

```cpp
tester::expect_eq(report, "ft_split token count", expected, actual);
```

## Reporting Convention

Keep score displays consistent across terminal, JSON, and HTML output.

Any `X/Y` score must mean `passed/total`.

Examples:

| Score | Meaning |
| --- | --- |
| `13/13` | All 13 checks passed. |
| `9/10` | 9 checks passed out of 10 total checks. |
| `1/10` under `Failed` | 1 check failed out of 10 total checks. |

Do not use `X/Y` to compare status types such as `OK/MOK` or `NOK/MNOK`. Use
plain counters instead, for example `OKx5`, `MOKx4`, and `MNOKx1`.

When output changes affect terminal summaries, `--review`, config-file
behavior, or HTML filters, extend `tester/driver/self_test.cpp` so future UI
work cannot silently regress the public output.

## Good Test Cases

Prefer cases that prove a different behavior:

- empty input;
- boundary sizes;
- truncation;
- first/last match;
- missing match;
- zero-length operations;
- unsigned byte behavior;
- malloc failure for allocating functions;
- cleanup after partial allocation failure.

Avoid adding five values that all prove the same thing. A tester gets stronger
when cases are different, not when they are noisy.

## Malloc Failure Tests

Use:

```cpp
malloc_fail_on(1);
result = ft_strdup("libft");
malloc_fail_off();
tester::expect_malloc(report, "ft_strdup malloc failure", result, false);
```

Always call `malloc_fail_off()` immediately after the function being tested.

For multi-allocation functions such as `ft_split` and `ft_lstmap`, test more
than one failure point and verify cleanup when possible.

## Pseudo-Random Tests

Use `tester::random_engine()` so `--seed` can reproduce the run:

```cpp
std::mt19937 &rng = tester::random_engine();
```

Keep random tests bounded and meaningful. They should explore edge combinations,
not make the tester slow for no reason.

## Updating Coverage

When a test adds meaningful coverage, update:

- `tester/src/coverage.cpp`
- `docs/COVERAGE.md`

The docs can be regenerated with:

```sh
make build
./libft_tester --coverage-md > docs/COVERAGE.md
```

## Updating Hints

Hints live in:

```text
tester/src/hints.cpp
```

A good hint is short, specific, and explains the most likely mistake without
pretending to know the exact bug.

## Validation Checklist

Before opening a pull request, run:

```sh
make build
./libft_tester --root /path/to/libft --summary-only --seed 42
./libft_tester --root /path/to/libft --json --no-color --only ft_strlen --seed 42 > libft-test-report.json
./libft_tester --root /path/to/libft --html --no-color --only ft_strlen --seed 42 > libft-test-report.html
./libft_tester --coverage
```

If the change touches allocation behavior, also run:

```sh
LIBFT_TESTER_NO_FORK=1 valgrind --leak-check=full \
  --show-leak-kinds=all --track-origins=yes \
  --errors-for-leak-kinds=all --error-exitcode=42 \
  ./libft_tester --root /path/to/libft --only ft_split --no-color
```
