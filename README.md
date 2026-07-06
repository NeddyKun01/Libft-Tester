# Libft Tester

A C++ tester for 42 `libft` projects, focused on:

- per-function coverage;
- compact and readable output;
- isolation with `fork`;
- detection of `SEGV`, `BUS`, `ABRT`, `FPE`, and `TIMEOUT`;
- `malloc` failure simulation;
- leak-checking mode with `valgrind`;
- support for terminals without color using `NO_COLOR=1`;
- filters by suite or function;
- JSON output for scripts and CI;
- version command to identify tester builds.

## How to Use

From the root of the tester repository:

```sh
make
```

Or from the parent folder, if the repository is inside a subfolder called `tester`:

```sh
make -C tester
```

You can also pass arguments through the Makefile:

```sh
make ARGS="--only ft_split"
make ARGS="--suite memory --verbose"
```

## Options

```sh
./libft_tester --help
./libft_tester --version
./libft_tester --list
./libft_tester --only ft_split
./libft_tester --suite memory
./libft_tester --timeout 5000
./libft_tester --verbose
./libft_tester --quiet
./libft_tester --json
./libft_tester --no-color
```

Makefile shortcuts:

```sh
make help
make list
make verbose
make json
make ci
```

For clean JSON output from the root:

```sh
make --no-print-directory -C tester json ARGS="--only ft_strlen"
```

## CI

```sh
make ci
make ci ARGS="--only ft_strlen"
```

The `ci` target uses colorless JSON output, so the output remains stable in GitHub Actions, GitLab CI, or local scripts.

## Leaks

```sh
make leaks
```

Leak mode runs without `fork`, so `valgrind` can analyze the tested functions directly instead of the runner's internal mechanism.

It also accepts filters:

```sh
make leaks ARGS="--only ft_split"
make leaks ARGS="--suite lists"
```

## Without Colors

```sh
NO_COLOR=1 make
```

## Statuses

- `OK`: normal test passed.
- `MOK`: `malloc`-related expectation passed.
- `NOK`: normal test failed.
- `MNOK`: `malloc`-related expectation failed.
- `SEGV`: the suite crashed with a segmentation fault.
- `BUS`: the suite crashed with a bus error.
- `ABRT`: the suite aborted.
- `FPE`: the suite crashed with an arithmetic error.
- `TIMEOUT`: the suite took too long to finish.

## Behavior

The tester does not automatically delete `libft.a` after running. This prevents race conditions and issues in CI or when multiple commands are executed in parallel.

To clean everything, use:

```sh
make fclean
```

## Structure

- `include/tester.hpp`: runner, asserts, output, and helpers.
- `include/test_modules.hpp`: suite declarations.
- `include/malloc_fail.hpp`: simulated `malloc` failure control.
- `src/*_tests.cpp`: tests separated by family.
- `src/malloc_fail.cpp`: `malloc` wrapper.
- `src/main.cpp`: suite registration and execution.

## Note

This tester avoids testing undefined C behavior when it is not required by the subject. For example, it does not test calls such as `ft_strlen(NULL)`.
