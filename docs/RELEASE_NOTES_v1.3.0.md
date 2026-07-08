# Libft Tester v1.3.0

This release removes the shell-script dependency from the tester workflow and
moves the project to a standalone C++ driver.

## Highlights

- `./libft_tester` is now a standalone C++ driver.
- The driver handles:
  - interactive menu;
  - smart run;
  - diagnose mode;
  - rescue mode;
  - self-test.
- Shell helper scripts were removed.
- Real Libft behavior tests now run through an internal suite binary:
  - `build/libft_suite`
- Rescue checks run through:
  - `build/rescue/libft_suite_rescue`
- The driver can still diagnose broken target projects before the target
  `libft.h` is safe to compile against.

## Why It Matters

Some school or shared machines can make shell scripts annoying to run because
of permissions, execution policies, or environment differences.

Before this release, `make` delegated important behavior to scripts under
`scripts/`.

Now the default flow is simpler:

```sh
make
```

`make` builds `./libft_tester`, and `./libft_tester` handles the rest.

## Architecture

The tester is now split into two layers:

```text
./libft_tester
  standalone C++ driver, does not link directly against the target Libft

build/libft_suite
  internal test suite, built only when real function tests are needed
```

This keeps diagnose, menu, smart run, and self-test available even when the
target project is incomplete or broken.

## Validation

This release was validated with:

```sh
./libft_tester --root /path/to/libft --summary-only --seed 42
./libft_tester --root /path/to/libft --rescue --seed 42
./libft_tester --root /path/to/libft --coverage
make self-test
```

GitHub Actions passed:

- tester self-test;
- build and test;
- coverage metadata;
- Valgrind leaks.
