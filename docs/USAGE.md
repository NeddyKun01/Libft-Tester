# Usage Guide

Libft Tester has two workflows:

- use `./libft_tester` for the interactive menu, direct commands, scripts, and CI;
- use `make` as a small convenience wrapper when you want it.

If you are unsure, start with the menu.

`./libft_tester` is a standalone C++ driver. It can run the menu, diagnose a
broken target project, and explain fallback results before the target library is
healthy enough to link. When real function tests are needed, it builds an
internal suite under `tester/build/`.

Documentation commands such as `--coverage`, `--explain`, and `--hint` do not
need the target project to build successfully.

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
make build
./libft_tester --root ../libft
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
make build
./libft_tester --root ..
```

## Menu Workflow

`./libft_tester --root ../libft` opens the menu when used from a normal
terminal.

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
| `10) Review summary` | Print compact reviewer-friendly output. |

If the driver runs without an interactive terminal, it falls back to smart test
instead of waiting for keyboard input. This keeps CI and scripts safe.

## Minimal Make Commands

| Command | Purpose |
| --- | --- |
| `make build` | Build the standalone `./libft_tester` driver. |
| `make ROOT_DIR=../libft` | Build, then open the menu as a shortcut. |
| `make self-test` | Validate the tester's own fallback behavior. |
| `make clean` | Remove tester build files and reports. |
| `make fclean` | Same as `clean`. |
| `make re` | Rebuild the driver. |

## Direct CLI Workflow

Build the driver first:

```sh
make build
```

Then run the binary. Commands that need real tests build the internal suite
automatically:

```sh
./libft_tester --root ../libft --summary-only
./libft_tester --root ../libft --only ft_split
./libft_tester --root ../libft --suite memory
./libft_tester --root ../libft --profile quick
./libft_tester --root ../libft --profile strict
./libft_tester --root ../libft --profile brutal --seed 42
./libft_tester --root ../libft --repeat 10 --seed 42
./libft_tester --root ../libft --fail-fast
./libft_tester --root ../libft --verbose
./libft_tester --root ../libft --review --seed 42
./libft_tester --root ../libft --json --no-color
./libft_tester --root ../libft --html --no-color
./libft_tester --explain ft_lstmap
./libft_tester --hint ft_split
./libft_tester --coverage
./libft_tester --root ../libft --doctor
./libft_tester --help
```

## Optional Config File

You can store common defaults in `.libft-tester.json`.

Example:

```json
{
  "root": "../libft",
  "profile": "strict",
  "seed": 42,
  "no_color": false
}
```

The tester looks for this file in the current working directory and in the
tester directory. You can also pass a specific file:

```sh
./libft_tester --config .libft-tester.json --review
```

CLI arguments always override config values.

See [`docs/examples/libft-tester.example.json`](examples/libft-tester.example.json)
for a ready-to-copy example.

## Doctor Mode

Doctor mode checks the tester environment without running the full suite.

It reports:

- required tools: `make`, `c++`, `cc`, `ar`, `nm`;
- optional tools: `valgrind`;
- target files: `Makefile`, `libft.h`, `libft.a`;
- tester internals such as templates and the internal suite path.

Missing optional tools or a missing internal suite are warnings. Missing
required tools or target files are errors.

For each problem, doctor mode prints a short `Fix:` hint. At the end, it also
prints a `Next action:` line so new users know what to run next.

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
./libft_tester --root ../libft --json --no-color > libft-test-report.json
./libft_tester --root ../libft --html --no-color > libft-test-report.html
```

The terminal output, JSON report, and HTML report all use the same score
direction: `passed/total`.

Examples:

| Display | Meaning |
| --- | --- |
| `13/13` | 13 checks passed out of 13 total checks. |
| `9/10` | 9 checks passed out of 10 total checks. |
| `1/10` under `Failed` | 1 check failed out of 10 total checks. |

Status counters such as `OKx5`, `MOKx4`, and `MNOKx1` are plain counters, not
ratios. They show how many checks produced each status.

The HTML report also includes:

- a score guide explaining `passed/total`;
- quick filters for all, failed, passed, malloc-related, and crash-related
  functions;
- a failure summary with links to failing function cards;
- likely-fix hints near the failure summary;
- copyable rerun commands for focused debugging.

## Valgrind

Use the menu option:

```text
7) Leak check
```

Or run Valgrind manually through the driver:

```sh
LIBFT_TESTER_NO_FORK=1 valgrind --trace-children=yes --leak-check=full \
  --show-leak-kinds=all --track-origins=yes \
  --errors-for-leak-kinds=all --error-exitcode=42 \
  ./libft_tester --root ../libft --only ft_split --no-color
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
