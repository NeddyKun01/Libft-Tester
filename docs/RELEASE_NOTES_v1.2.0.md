# Libft Tester v1.2.0

This release focuses on making the tester easier to use, easier to understand,
and easier to recommend to other people.

## Highlights

- `make` now opens an interactive menu when used in a terminal.
- Advanced options are still available through `./libft_tester`.
- The menu shows project health before running tests:
  - `Makefile`
  - `libft.h`
  - `libft.a`
  - tester runner
- The menu recommends the next useful action based on the project state.
- The UI was polished with clearer sections, colors, status labels, and a
  visible tester version.
- Public documentation was rewritten in English for both 42 and non-42 users.
- Issue templates, contribution docs, troubleshooting docs, and diagnosis docs
  were cleaned up for clearer onboarding.

## Why It Matters

Earlier versions were powerful, but still asked users to remember too many
`make` targets or CLI flags. That is fine for automation, but not ideal for
someone opening the tester for the first time.

Now the default workflow is:

```sh
make
```

From there, the tester guides the user through normal tests, strict tests,
diagnostics, rescue mode, reports, and help.

The CLI remains available for scripts, CI, and advanced usage:

```sh
make build
./libft_tester --summary-only --seed 42
```

## User Experience

The menu now gives quick answers before running anything:

```text
Makefile:  OK
libft.h:   OK
libft.a:   OK
runner:    OK

Recommended: Full test
Project shape looks ready for the full suite.
```

If something is broken, the menu points the user toward diagnose or rescue mode
instead of leaving them with a raw build error.

## Documentation

The documentation was refreshed so that a new user can understand:

- what the tester does;
- what files a target project needs;
- how to run the tester next to a project or inside one;
- when to use the menu;
- when to use the CLI;
- how to read `OK`, `MOK`, `NOK`, and `MNOK`;
- how diagnose and rescue modes help with broken projects;
- how to contribute new tests or documentation.

## Validation

This release was validated with:

```sh
make ROOT_DIR=/path/to/libft build
make ROOT_DIR=/path/to/libft ARGS="--summary-only --seed 42"
./libft_tester --summary-only --seed 42
./libft_tester --coverage
make self-test
```

GitHub Actions passed after the documentation and release-preparation updates.
