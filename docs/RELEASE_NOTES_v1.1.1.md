# Libft Tester v1.1.1

This release improves the default user experience when a Libft project is
broken or incomplete.

## Highlights

- `make` is now a smart entrypoint.
- `make normal` keeps the old pure build-and-test behavior.
- When the normal tester cannot finish, the smart flow now runs:
  - `diagnose`
  - `rescue-test`, when `libft.a` exists
  - a final health summary
- The README was rewritten as a clearer public usage guide.
- GitHub Actions were updated to remove Node.js deprecation noise.
- Self-tests now cover:
  - partial Libft projects
  - broken Makefiles
  - missing archives
  - rescue fallback behavior

## Why It Matters

Before this release, a user running only:

```sh
make
```

could hit a hard failure before seeing useful feedback, especially when
`libft.h` or the Libft `Makefile` was broken.

Now the default flow tries to help:

```text
normal test
if it cannot finish:
  diagnose
  rescue-test when possible
  final health summary
```

The tester still exits with an error when the project is broken, but it gives a
clearer explanation and tests any valid functions it can safely reach.

## Example Summary

```text
Final Health Summary
------------------------------------------------------------
Normal tests: FAILED
Diagnose: PROBLEMS FOUND
Rescue test: OK

Problem counters:
- structure/build errors: 0
- missing source files: 12
- missing header declarations: 0
- missing library symbols: 12

Rescue counters:
- real symbols found: 31
- missing symbols skipped: 12
- failed tested functions: 0
```

## Validation

This release was validated with:

```sh
make self-test
make ROOT_DIR=/path/to/libft ARGS="--summary-only --seed 42"
make ROOT_DIR=/path/to/libft normal ARGS="--summary-only --seed 42"
```

GitHub Actions also passed the main tester, self-test, Valgrind leak check, and
coverage metadata jobs.
