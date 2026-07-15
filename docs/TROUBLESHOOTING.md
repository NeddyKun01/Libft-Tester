# Troubleshooting

This guide lists common setup and testing problems.

## Start With The Menu

For most problems, run:

```sh
./libft_tester --root ../libft
```

Then choose:

```text
5) Diagnose project
```

If `libft.a` exists and the project is incomplete, choose:

```text
6) Rescue test
```

Diagnose explains structure problems. Rescue tests real symbols that already
exist in `libft.a`.

## `ROOT_DIR` Does Not Exist

The tester could not find the target directory you passed.

Use an absolute path if you are unsure:

```sh
./libft_tester --root /path/to/libft
```

You can also change it from the menu:

```text
r) Change ROOT_DIR
```

## `libft.h` Was Not Found

`ROOT_DIR` must point to the root of the target project, not to the tester
folder or to a subdirectory.

Expected layout:

```text
libft/
├── Makefile
├── libft.h
└── ft_*.c
```

## `libft.a` Is Missing

The tester builds the target project before linking `./libft_tester`. If
`libft.a` is missing, check that the target `Makefile` builds the library with
the default `make` target.

Use menu option `5) Diagnose project` to see the build error.

## A Function Shows `NOK`

Build the driver:

```sh
make build
```

Run only that function in verbose mode:

```sh
./libft_tester --root ../libft --only ft_split --verbose
```

Then ask for hints:

```sh
./libft_tester --hint ft_split
```

If the failing case is random, repeat it with the printed seed:

```sh
./libft_tester --root ../libft --only ft_strlen --seed 42
```

## Use `Debug Focus`

When one or more functions fail, the tester prints a `Debug Focus` block after
the failure details.

Copy the first suggested command, fix that function, then rerun the same seed.
This keeps the feedback loop small and reproducible:

```text
Debug Focus
  failed functions: ft_striteri, ft_putstr_fd, ft_putendl_fd, ft_lstmap
  try next:
    ./libft_tester --root '../libft' --only ft_striteri --verbose --seed 42
    ./libft_tester --root '../libft' --only ft_putstr_fd --verbose --seed 42
    ./libft_tester --root '../libft' --only ft_putendl_fd --verbose --seed 42
    ... and 1 more failed function(s)
    ./libft_tester --hint ft_striteri
```

Prefer fixing one function at a time. After it passes, run the full tester
again to see what remains.

## A Function Shows `MNOK`

`MNOK` means a malloc-related expectation failed.

Common causes:

- the function does not return `NULL` when allocation fails;
- previous allocations are not freed after a later allocation fails;
- the result is not allocated when it should be.

Use menu option `7) Leak check`, or run Valgrind manually:

```sh
LIBFT_TESTER_NO_FORK=1 valgrind --leak-check=full \
  --show-leak-kinds=all --track-origins=yes \
  --errors-for-leak-kinds=all --error-exitcode=42 \
  ./libft_tester --root ../libft --only ft_split --no-color
```

## A Suite Shows `SEGV`, `BUS`, `ABRT`, Or `FPE`

The suite crashed. Use a focused run:

```sh
./libft_tester --root ../libft --suite strings --verbose --fail-fast
```

If needed, run under a debugger after building:

```sh
gdb --args ./libft_tester --root ../libft --only ft_split
```

## A Suite Shows `TIMEOUT`

The function may have an infinite loop or may be much slower than expected.

Run only the related suite with a larger timeout:

```sh
./libft_tester --root ../libft --suite lists --timeout 10000 --verbose
```

## GitHub Actions Does Not Run The Real Tests

Check that the tester repository has this Actions variable:

```text
LIBFT_REPOSITORY
```

Example value:

```text
OWNER/Libft
```

Without that variable, the workflow shows a configuration-help job instead of
testing a target repository.

## GitHub Actions Cannot Checkout The Target Repository

Possible causes:

- the `LIBFT_REPOSITORY` value is misspelled;
- the target repository is private and the workflow token cannot access it;
- the repository does not exist under that owner.

For private repositories, use a token with access to both repositories or keep
the target project public while testing.
