# Usage Guide

This guide explains the two intended ways to use Libft Tester:

- `make` for the interactive menu;
- `./libft_tester` for advanced direct CLI usage.

## Repository Layouts

The tester can live next to your Libft repository:

```text
projects/
├── libft/
└── Libft-Tester/
```

```sh
cd Libft-Tester
make ROOT_DIR=../libft
```

It can also live inside your Libft repository:

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

## Interactive Menu

`make` opens the menu when used from a normal terminal.

The menu gives access to:

- smart test;
- quick test;
- full test;
- strict test;
- diagnose project;
- rescue test;
- leak check;
- function explain/hint;
- HTML report generation;
- advanced CLI help.

If `make` is executed without an interactive terminal, it falls back to the
smart test instead of waiting for keyboard input. This keeps CI and scripts safe.

## Minimal Make Commands

| Command | Purpose |
| --- | --- |
| `make ROOT_DIR=../libft` | Open the menu. |
| `make ROOT_DIR=../libft build` | Build `./libft_tester`. |
| `make self-test` | Validate the tester's own fallback behavior. |
| `make clean` | Remove tester build files and reports. |
| `make fclean` | Same as `clean`. |
| `make re ROOT_DIR=../libft` | Rebuild the runner. |

## Advanced CLI

Build first:

```sh
make ROOT_DIR=../libft build
```

Then run the binary:

```sh
./libft_tester --summary-only
./libft_tester --only ft_split
./libft_tester --suite memory
./libft_tester --profile quick
./libft_tester --profile strict
./libft_tester --profile brutal --seed 42
./libft_tester --repeat 10 --seed 42
./libft_tester --fail-fast
./libft_tester --verbose
./libft_tester --json --no-color
./libft_tester --html --no-color
./libft_tester --explain ft_lstmap
./libft_tester --hint ft_split
./libft_tester --coverage
./libft_tester --help
```

## Profiles

| Profile | Repeats | Timeout | Fail-fast | Best for |
| --- | --- | --- | --- | --- |
| `quick` | 1 | 1500 ms | yes | fast checks while coding |
| `normal` | 1 | 3000 ms | no | regular local testing |
| `strict` | 10 | 6000 ms | no | deeper validation before push |
| `brutal` | 25 | 9000 ms | no | final stress run before release |

## Reports

Use the menu option:

```text
9) Generate HTML report
```

Or use the CLI directly:

```sh
./libft_tester --json --no-color > libft-test-report.json
./libft_tester --html --no-color > libft-test-report.html
```

## Valgrind

Use the menu option:

```text
7) Leak check
```

Or run Valgrind manually after building:

```sh
LIBFT_TESTER_NO_FORK=1 valgrind --leak-check=full \
  --show-leak-kinds=all --track-origins=yes \
  --errors-for-leak-kinds=all --error-exitcode=42 \
  ./libft_tester --only ft_split --no-color
```

## GitHub Actions

Set the repository variable `LIBFT_REPOSITORY` in the tester repository:

```text
NeddyKun01/Libft
```

After that, pushes to the tester repository run:

- the main JSON suite;
- a standalone HTML report artifact;
- a focused Valgrind leak check;
- coverage metadata generation;
- the tester self-test.

You can also run the workflow manually with `workflow_dispatch` and provide an
`owner/repo` value.
