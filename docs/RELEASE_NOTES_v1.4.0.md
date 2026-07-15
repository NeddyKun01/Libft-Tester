# Libft Tester v1.4.0

This release focuses on maintainability, clearer failure output, and faster
local validation.

## Highlights

- Moved tester source code under `tester/` so the repository root stays focused
  on documentation, metadata, and the main `Makefile`.
- Reorganized tests into `tester/tests/` with one `.cpp` file per Libft
  function.
- Added `tester/tests/suites.cpp` to group function tests into public suites.
- Added `tester/tests/test_helpers.hpp` for shared test-only helpers.
- Improved rescue failures so details print directly in the terminal instead
  of pointing users to per-function `.log` files.
- Added per-function and per-case isolation for high-risk checks, so crashes
  point to cases such as `ft_putstr_fd null` instead of vague suite names.
- Reduced `make self-test` time by using a minimal self-test suite and a
  separate `tester/build-self-test/` directory.

## Why It Matters

Contributors can now open the exact file they care about, such as:

```text
tester/tests/ft_split.cpp
tester/tests/ft_lstmap.cpp
tester/tests/ft_putstr_fd.cpp
```

Users get clearer terminal output when a function crashes, and maintainers get
faster feedback from the self-test.

## Validation

This release was validated locally with:

```sh
make build
make self-test
./libft_tester --root /path/to/libft --seed 42
```

The final `make self-test` run completed with:

```text
Self-test failures: 0
```
