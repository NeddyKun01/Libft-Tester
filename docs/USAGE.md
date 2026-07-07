# Usage Guide

This guide explains how to use Libft Tester in day-to-day development, deeper
validation, and GitHub Actions.

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

## Common Commands

Run everything:

```sh
make ROOT_DIR=../libft
```

Run a single function:

```sh
make ROOT_DIR=../libft ARGS="--only ft_split"
```

Run a suite:

```sh
make ROOT_DIR=../libft ARGS="--suite memory"
```

Show only the final summary:

```sh
make ROOT_DIR=../libft summary
```

Run with a reproducible seed:

```sh
make ROOT_DIR=../libft ARGS="--seed 42"
```

Diagnose project-structure problems before running the full tester:

```sh
make ROOT_DIR=../libft diagnose
```

Test only functions that have real symbols in `libft.a`:

```sh
make ROOT_DIR=../libft rescue-test
```

Validate the tester's own diagnose/rescue behavior:

```sh
make self-test
```

## Profiles

Profiles are shortcuts for different levels of confidence.

| Profile | Repeats | Timeout | Fail-fast | Best for |
| --- | --- | --- | --- | --- |
| `quick` | 1 | 1500 ms | yes | fast checks while coding |
| `normal` | 1 | 3000 ms | no | regular local testing |
| `strict` | 10 | 6000 ms | no | deeper validation before push |
| `brutal` | 25 | 9000 ms | no | final stress run before release |

Examples:

```sh
make ROOT_DIR=../libft quick ARGS="--only ft_split"
make ROOT_DIR=../libft strict
make ROOT_DIR=../libft brutal ARGS="--summary-only --seed 42"
```

The shortcut `--strict` is the same idea as `--profile strict`:

```sh
./libft_tester --strict --summary-only
```

Manual options override profile defaults:

```sh
./libft_tester --profile brutal --repeat 3
```

## Reports

Generate JSON for scripts and CI:

```sh
make ROOT_DIR=../libft report
```

Generate a standalone HTML report:

```sh
make ROOT_DIR=../libft report-html
```

Direct CLI usage:

```sh
./libft_tester --json --no-color > libft-test-report.json
./libft_tester --html --no-color > libft-test-report.html
```

## Hints And Coverage

Show what one function is expected to cover:

```sh
make ROOT_DIR=../libft explain FUNC=ft_lstmap
```

Show debugging hints:

```sh
make ROOT_DIR=../libft hint FUNC=ft_split
```

Show the documented coverage table:

```sh
make ROOT_DIR=../libft coverage
```

Regenerate the coverage documentation from code metadata:

```sh
make ROOT_DIR=../libft coverage-docs
```

## Valgrind

Run a focused Valgrind check:

```sh
make ROOT_DIR=../libft leaks ARGS="--only ft_split --no-color"
```

The leak target disables fork isolation internally so Valgrind can inspect the
tested code more directly.

## GitHub Actions

Set the repository variable `LIBFT_REPOSITORY` in the tester repository:

```text
NeddyKun01/Libft
```

After that, pushes to the tester repository run:

- the main JSON suite;
- a standalone HTML report artifact;
- a focused Valgrind leak check;
- coverage metadata generation.

You can also run the workflow manually with `workflow_dispatch` and provide an
`owner/repo` value.
