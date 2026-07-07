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
