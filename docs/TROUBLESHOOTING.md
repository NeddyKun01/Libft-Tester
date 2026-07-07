# Troubleshooting

This guide lists common setup and testing problems.

## Start With Diagnose

If the tester cannot build, run:

```sh
make ROOT_DIR=../libft diagnose
```

This checks the student's `Makefile`, `libft.h`, source files, `libft.a`, and
library symbols without compiling the C++ runner against a broken header.

If the project is incomplete but `libft.a` exists, you can still test the real
symbols that are present:

```sh
make ROOT_DIR=../libft rescue-test
```

## `ROOT_DIR` Does Not Exist

The tester could not find the Libft directory you passed.

Use an absolute path if you are unsure:

```sh
make ROOT_DIR=/home/user/projects/libft
```

## `libft.h` Was Not Found

`ROOT_DIR` must point to the root of a Libft project, not to the tester folder
or to a subdirectory.

Expected layout:

```text
libft/
├── Makefile
├── libft.h
└── ft_*.c
```

## `libft.a` Is Missing

The tester runs `make -C $(ROOT_DIR)` before linking. If `libft.a` is missing,
check that the Libft `Makefile` builds the library with the default `make`
target.

## A Function Shows `NOK`

Run only that function in verbose mode:

```sh
make ROOT_DIR=../libft ARGS="--only ft_split --verbose"
```

Then ask the tester for hints:

```sh
make ROOT_DIR=../libft hint FUNC=ft_split
```

If the failing case is random, repeat it with the printed seed:

```sh
make ROOT_DIR=../libft ARGS="--only ft_strlen --seed 42"
```

## A Function Shows `MNOK`

`MNOK` means a malloc-related expectation failed.

Common causes:

- the function does not return `NULL` when allocation fails;
- previous allocations are not freed after a later allocation fails;
- the result is not allocated when it should be.

Run a focused leak check:

```sh
make ROOT_DIR=../libft leaks ARGS="--only ft_split --no-color"
```

## A Suite Shows `SEGV`, `BUS`, `ABRT`, Or `FPE`

The suite crashed. Use a focused run:

```sh
make ROOT_DIR=../libft ARGS="--suite strings --verbose --fail-fast"
```

If needed, run under a debugger from the tester directory after building:

```sh
make ROOT_DIR=../libft help
gdb --args ./libft_tester --only ft_split
```

## A Suite Shows `TIMEOUT`

The function may have an infinite loop or may be much slower than expected.

Run only the related suite with a larger timeout:

```sh
make ROOT_DIR=../libft ARGS="--suite lists --timeout 10000 --verbose"
```

## GitHub Actions Does Not Run The Real Tests

Check that the tester repository has this Actions variable:

```text
LIBFT_REPOSITORY
```

Example value:

```text
NeddyKun01/Libft
```

Without that variable, the workflow shows a configuration-help job instead of
testing a Libft repository.

## GitHub Actions Cannot Checkout The Libft Repository

Possible causes:

- the `LIBFT_REPOSITORY` value is misspelled;
- the Libft repository is private and the workflow token cannot access it;
- the repository does not exist under that owner.

For private repositories, use a token with access to both repositories or keep
the Libft project public while testing.

## The Node.js Warning In GitHub Actions

GitHub may show a warning about official actions moving from Node.js 20 to a
newer runtime. This is not a tester failure if all jobs are green.
