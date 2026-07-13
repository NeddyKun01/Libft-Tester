# Libft Tester v1.6.0

This release focuses on report clarity and confidence. Scores are now easier to
read, the HTML report is more useful when debugging failures, and the tester has
stronger self-tests to protect the terminal and HTML output contracts.

## Highlights

- Added a `Score Guide` section to the HTML report.
- Made every score display consistently use `passed/total`.
- Renamed function score headers to `OK/Total`.
- Replaced ambiguous summary ratios such as `OK/MOK` with explicit `Passed`
  and `Failed` metrics.
- Added quick HTML filters for:
  - all functions;
  - failed functions;
  - passed functions.
- Added filter metadata to function table rows and function cards.
- Added visual README previews for the interactive menu and HTML report.
- Updated README, usage docs, and contribution docs with the reporting
  convention.
- Added self-test coverage for terminal output and HTML report structure.

## Why It Matters

The tester should be readable at a glance. Before this release, a value such as
`OK/MOK 5/4` could look like a score, even though it was really comparing two
different status counters. That could confuse users trying to understand if a
function passed or failed.

From this release onward, any `X/Y` value means one thing only:

```text
passed checks / total checks
```

Examples:

| Display | Meaning |
| --- | --- |
| `13/13` | 13 checks passed out of 13 total checks. |
| `9/10` | 9 checks passed out of 10 total checks. |
| `1/10` under `Failed` | 1 check failed out of 10 total checks. |

Status values such as `OKx5`, `MOKx4`, and `MNOKx1` are plain counters, not
ratios.

## HTML Report

The HTML report now includes:

- a top-level score guide;
- `Passed` and `Failed` cards using `passed/total` and `failed/total`;
- `All`, `Failed`, and `Passed` filters;
- a failed-function summary with links;
- copyable rerun commands for focused debugging;
- collapsible function cards.

Example focused HTML command:

```sh
./libft_tester --root ../libft --only ft_lstmap --html --no-color --seed 42 > libft-test-report.html
```

## Terminal Output Contract

The terminal output now has self-test coverage for:

- the `OK/Total` score header;
- the `passed/total` score direction;
- absence of the old `Score` header;
- the order of debugging sections:
  `Results`, `Failure Details`, `Debug Focus`, `Summary`.

This makes future UI work safer because accidental output regressions are more
likely to be caught by `make self-test`.

## Validation

This release was validated locally with:

```sh
make build
make self-test
./libft_tester --root /home/neddykun/Secretária/reposdeteste/git_libft --only ft_lstmap --html --no-color --seed 42
git diff --check
```

The final `make self-test` run completed with:

```text
Self-test failures: 0
```
