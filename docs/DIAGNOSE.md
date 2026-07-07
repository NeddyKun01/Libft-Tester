# Diagnose Mode

`diagnose` is a structural checker for Libft projects.

It is meant for the situation where the normal tester cannot even build because
the student's `Makefile`, `libft.h`, or archive contents are incomplete.

## Run It

```sh
make ROOT_DIR=../libft diagnose
```

Unlike the normal tester, this command does not need to compile the C++ test
runner against the student's `libft.h`. It is a shell-based diagnostic pass.

## What It Checks

- `ROOT_DIR` exists.
- `Makefile` exists.
- `libft.h` exists.
- `make` can build the project.
- `libft.a` is produced.
- `libft.h` declares expected functions.
- each expected `ft_*.c` source file exists.
- `libft.a` contains expected symbols.

## Why It Helps

Normal testing is strict: if the project cannot build, it should fail.

`diagnose` is different. It tries to explain why the project cannot build or why
some functions cannot be tested:

```text
ft_split          yes     no      yes     MISSING_HEADER   add prototype: char **ft_split(char const *s, char c);
ft_lstmap         yes     yes     no      MISSING_SYMBOL   add ft_lstmap.c to SRCS/Makefile
```

This separates implementation bugs from project-structure bugs.

## Model Files

The tester includes two known-good reference files:

```text
templates/libft.h
templates/Makefile
```

They are not copied automatically and they do not hide mistakes. They exist so a
student can compare their project against a clean expected structure.

## Important

`diagnose` is not a replacement for the normal tester.

Use it first when the project is structurally broken. Once the header, Makefile,
and archive are fixed, run:

```sh
make ROOT_DIR=../libft
```

## Rescue Test

If you want to test every function that is actually present in `libft.a`, even
when other functions are missing, use:

```sh
make ROOT_DIR=../libft rescue-test
```

`rescue-test` builds a special runner with:

- `templates/libft.h` instead of the student's possibly broken header;
- weak fallback stubs for missing functions;
- `libft.a` linked with `--whole-archive` so real symbols override stubs.

Then it runs tests only for functions whose real symbols are present in
`libft.a`. Missing symbols are reported as `SKIP`.

Some functions may also be reported as indirect coverage. For example,
`ft_lstdelone` is exercised through list cleanup paths, but it does not have a
standalone direct rescue check.

This does not make the project valid. It only helps answer a practical question:

```text
Which existing functions can still be tested right now?
```

Use `diagnose` to fix the project structure and the normal tester for final
validation.

## Tester Self-Test

The tester also has a small self-test suite that creates temporary broken Libft
fixtures under `/tmp`:

```sh
make self-test
```

It checks that:

- `diagnose` fails and explains a missing `libft.h`;
- `diagnose` detects a partial `libft.a`;
- `rescue-test` runs an existing `ft_strlen`;
- `rescue-test` skips missing symbols such as `ft_split`.
