# Libft Tester v1.5.0

This release focuses on day-to-day usability: clearer menu guidance, better
failure triage, and documentation commands that work even when the target Libft
project is broken.

## Highlights

- Redesigned the interactive menu header with:
  - compact target health;
  - recommended menu key;
  - matching CLI command for the current `ROOT_DIR`.
- Added `Debug Focus` after failing runs so users get copy/paste-ready commands
  for the first failed functions.
- Included the active `--root` and `--seed` in suggested debug commands.
- Made `--coverage`, `--coverage-md`, `--explain`, and `--hint` run directly
  from `./libft_tester` without building or linking the target project.
- Isolated internal build folders by `ROOT_DIR`, preventing target/build
  collisions when switching projects or running self-tests.
- Updated README and docs around the standalone `./libft_tester` workflow.
- Improved `--help` with practical examples for normal runs, focused debugging,
  strict validation, hints, and coverage.

## Why It Matters

New users can open the tester and immediately see what to do next. When a test
fails, they no longer need to guess the right command: the terminal gives a
focused, reproducible next step.

Example:

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

## Validation

This release was validated locally with:

```sh
make build
make self-test
./libft_tester --root /home/neddykun/Secretária/reposdeteste/git_libft --seed 42
./libft_tester --root /home/neddykun/Secretária/reposdeteste/git_libft --only ft_strlen --json --no-color --seed 42
./libft_tester --root /home/neddykun/Secretária/reposdeteste/git_libft --only ft_strlen --html --no-color --seed 42
./libft_tester --root /tmp/does-not-exist --explain ft_lstmap
```

The final `make self-test` run completed with:

```text
Self-test failures: 0
```
