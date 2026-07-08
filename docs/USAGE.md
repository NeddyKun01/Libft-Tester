# Usage Guide

Libft Tester has two workflows:

- use `make` for the interactive menu;
- use `./libft_tester` for direct commands, scripts, and CI.

If you are unsure, start with the menu.

`./libft_tester` is a standalone C++ driver. It can run the menu, diagnose a
broken target project, and explain fallback results before the target library is
healthy enough to link. When real function tests are needed, it builds an
internal suite at `build/libft_suite`.

Use `--doctor` when you want a quick environment check:

```sh
./libft_tester --root ../libft --doctor
```

## Project Layouts

The tester can live next to your target repository:

```text
projects/
├── libft/
└── Libft-Tester/
```

```sh
cd Libft-Tester
make ROOT_DIR=../libft
```

The tester can also live inside the target repository:

```text
libft/
├── Makefile
├── libft.h
├── ft_*.c
└── tester/
```

```sh
cd tester
make
```

## Menu Workflow

`make` opens the menu when used from a normal terminal.

Start with:

```text
1) Smart test
```

Smart test tries a normal run first. If the target project is structurally
broken, it falls back to diagnostics and rescue testing when possible.

Useful menu options:

| Option | Use |
| --- | --- |
| `1) Smart test` | Best first run. |
| `2) Quick test` | Fast feedback while editing code. |
| `3) Full test` | Normal full run. |
| `4) Strict test` | Stronger validation before sharing work. |
| `5) Diagnose project` | Explain Makefile/header/archive problems. |
| `6) Rescue test` | Test real symbols in `libft.a` even if the project is incomplete. |
| `7) Leak check` | Run a focused Valgrind check. |
| `8) Explain or hint a function` | Read coverage notes or debugging hints. |
| `d) Doctor environment` | Check required tools and target project shape. |
| `9) Generate HTML report` | Write a standalone HTML report. |

If `make` runs without an interactive terminal, it falls back to smart test
instead of waiting for keyboard input. This keeps CI and scripts safe.

## Minimal Make Commands

| Command | Purpose |
| --- | --- |
| `make ROOT_DIR=../libft` | Open the menu. |
| `make ROOT_DIR=../libft build` | Build the standalone `./libft_tester` driver. |
| `make self-test` | Validate the tester's own fallback behavior. |
| `make clean` | Remove tester build files and reports. |
| `make fclean` | Same as `clean`. |
| `make re ROOT_DIR=../libft` | Rebuild the driver. |

## Direct CLI Workflow

Build the driver first:

```sh
make ROOT_DIR=../libft build
```

Then run the binary. Commands that need real tests build the internal suite
automatically:

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
./libft_tester --doctor
./libft_tester --help
```

## Doctor Mode

Doctor mode checks the tester environment without running the full suite.

It reports:

- required tools: `make`, `c++`, `cc`, `ar`, `nm`;
- optional tools: `valgrind`;
- target files: `Makefile`, `libft.h`, `libft.a`;
- tester internals such as templates and the internal suite path.

Missing optional tools or a missing internal suite are warnings. Missing
required tools or target files are errors.

## Profiles

| Profile | Repeats | Timeout | Fail-fast | Best for |
| --- | --- | --- | --- | --- |
| `quick` | 1 | 1500 ms | yes | Fast checks while coding. |
| `normal` | 1 | 3000 ms | no | Regular local testing. |
| `strict` | 10 | 6000 ms | no | Deeper validation before sharing work. |
| `brutal` | 25 | 9000 ms | no | Final stress run before release. |

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

Or run Valgrind manually against the internal suite:

```sh
make ROOT_DIR=../libft suite
LIBFT_TESTER_NO_FORK=1 valgrind --leak-check=full \
  --show-leak-kinds=all --track-origins=yes \
  --errors-for-leak-kinds=all --error-exitcode=42 \
  build/libft_suite --only ft_split --no-color
```

## GitHub Actions

Set the repository variable `LIBFT_REPOSITORY` in the tester repository:

```text
owner/repository
```

Example:

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
