# Libft Tester v1.1.0

This release focuses on broken or incomplete Libft projects.

## Highlights

- Added `make diagnose` for structural project diagnostics.
- Added `make rescue-test` for partial testing of real symbols in `libft.a`.
- Added weak rescue stubs so incomplete libraries can still be linked safely.
- Added known-good reference files:
  - `templates/libft.h`
  - `templates/Makefile`
- Added `make self-test` with temporary broken Libft fixtures.
- Added a GitHub Actions self-test job for diagnose/rescue behavior.
- Added `docs/DIAGNOSE.md`.

## Why It Matters

Before this release, a broken `Makefile` or `libft.h` could stop the tester
before any useful function-level feedback appeared.

Now the tester can separate structural problems from implementation problems:

```sh
make ROOT_DIR=../libft diagnose
make ROOT_DIR=../libft rescue-test
```

`diagnose` explains what is structurally wrong.

`rescue-test` tests the functions that actually exist in `libft.a` and marks
missing symbols as `SKIP`.

## Validation

The new behavior is covered by `make self-test`, which creates broken temporary
fixtures under `/tmp` and checks that diagnose/rescue respond correctly.

