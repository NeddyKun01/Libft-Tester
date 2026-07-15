# Libft Tester v1.7.0

This release focuses on reviewer experience, richer HTML debugging, optional
configuration, and stronger output contracts.

## Highlights

- Added `--review` mode for short, shareable tester output.
- Added a menu entry for review summaries.
- Added HTML report filters with counters for:
  - all functions;
  - passed functions;
  - failed functions;
  - malloc-related functions;
  - crash-related functions.
- Added copyable rerun commands per failing function in the HTML report.
- Added a `Likely Fixes` section to the HTML report.
- Added optional `.libft-tester.json` support.
- Added a ready-to-copy config example in `docs/examples/`.
- Strengthened `make self-test` with review, config, and HTML v2 contracts.

## Review Mode

Review mode is meant for reviewers, evaluators, and quick sharing:

```sh
./libft_tester --root ../libft --review --seed 42
```

It prints:

- final verdict;
- score as `passed/total`;
- pass rate;
- status counters;
- failed functions;
- crash functions;
- malloc failures;
- focused rerun commands.

Example:

```text
Libft Tester Review
  verdict: FAIL
  score: 3061/3066 (99%) | profile: normal | seed: 42
  status: OKx3001 MOKx60 NOKx4 MNOKx1
  failed functions: ft_striteri, ft_putstr_fd, ft_putendl_fd, ft_lstmap
  crash functions: ft_striteri, ft_putstr_fd, ft_putendl_fd
  malloc failures: ft_lstmap

Reproduce first failures:
    ./libft_tester --root '../libft' --only ft_striteri --verbose --seed 42
    ./libft_tester --hint ft_striteri
```

## HTML Report v2

The HTML report now includes richer filtering and debugging actions:

- `All`, `Failed`, `Passed`, `Malloc`, and `Crash` filters;
- filter counters;
- function cards tagged by result and failure category;
- copyable rerun commands for failing functions;
- likely-fix hints near the failure summary.

Example:

```sh
./libft_tester --root ../libft --html --no-color > libft-test-report.html
```

## Optional Config File

You can add `.libft-tester.json` either in the tester directory or in the
current working directory.

Example:

```json
{
  "root": "../libft",
  "profile": "strict",
  "seed": 42,
  "no_color": false
}
```

CLI arguments always override config values:

```sh
./libft_tester --config .libft-tester.json --profile quick --review
```

## Validation

This release was validated locally with:

```sh
make build
make self-test
./libft_tester --version
./libft_tester --root /home/neddykun/Secretária/reposdeteste/git_libft --review --seed 42
./libft_tester --root /home/neddykun/Secretária/reposdeteste/git_libft --only ft_lstmap --html --no-color --seed 42
git diff --check
```

The final `make self-test` run completed with:

```text
Self-test failures: 0
```
