# Libft Tester v1.3.1

This release polishes the new C++ driver architecture introduced in v1.3.0 and
makes doctor mode more useful for first-time users.

## Highlights

- Split the C++ driver into smaller modules:
  - process/common helpers;
  - diagnose;
  - rescue and smart run;
  - menu;
  - self-test;
  - doctor mode.
- Added `./libft_tester --doctor`.
- Added doctor mode to the interactive menu.
- Added self-test coverage for doctor mode.
- Improved doctor output with:
  - per-problem `Fix:` hints;
  - a final `Next action:` recommendation.

## Why It Matters

The tester is now easier to maintain internally and easier to use externally.

Doctor mode gives quick answers before users run the full suite:

```sh
./libft_tester --root ../libft --doctor
```

It checks required tools, optional tools, target files, and tester internals.
When something is missing, it now explains what to fix instead of only saying
that something is missing.

## Example

```text
[ERROR] Makefile missing: ../libft/Makefile
        Fix: restore the target Makefile or compare with tester/templates/Makefile

Next action: fix required errors, then run
`./libft_tester --root ../libft --doctor` again.
```

## Validation

This release was validated locally with:

```sh
make build
make self-test
./libft_tester --root /path/to/libft --doctor
./libft_tester --root /path/to/libft --summary-only --seed 42
```

The self-test now includes doctor mode fixtures for:

- missing `ROOT_DIR`;
- missing `Makefile`;
- missing `libft.h`;
- missing `libft.a` as a warning-only state;
- valid target projects.
