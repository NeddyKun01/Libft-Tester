# Diagnose And Rescue Modes

Diagnose mode is a structural checker for target `libft` projects.

It is useful when the normal tester cannot build because the target `Makefile`,
`libft.h`, source files, or archive contents are incomplete.

## Run Diagnose

Run `make` and choose:

```text
5) Diagnose project
```

Unlike the normal tester, diagnose mode does not compile the C++ runner against
the target `libft.h`. It is a shell-based diagnostic pass, so it can still help
when the target header is broken.

## What Diagnose Checks

- `ROOT_DIR` exists.
- `Makefile` exists.
- `libft.h` exists.
- `make` can build the target project.
- `libft.a` is produced.
- `libft.h` declares expected functions.
- each expected `ft_*.c` source file exists.
- `libft.a` contains expected symbols.

## Example Output

```text
ft_split          yes     no      yes     MISSING_HEADER   add prototype: char **ft_split(char const *s, char c);
ft_lstmap         yes     yes     no      MISSING_SYMBOL   add ft_lstmap.c to SRCS/Makefile
```

This separates project-structure bugs from function-behavior bugs.

## Model Files

The tester includes two known-good reference files:

```text
templates/libft.h
templates/Makefile
```

They are references only. They are not copied automatically and they do not hide
mistakes in the target project.

## Run Rescue

If you want to test every function that is actually present in `libft.a`, even
when other functions are missing, run `make` and choose:

```text
6) Rescue test
```

Rescue mode builds a special runner with:

- `templates/libft.h` instead of the target project's possibly broken header;
- weak fallback stubs for missing functions;
- `libft.a` linked with `--whole-archive` so real symbols override stubs.

Then it runs tests only for functions whose real symbols are present in
`libft.a`. Missing symbols are reported as `SKIP`.

Some functions may also be reported as indirect coverage. For example,
`ft_lstdelone` is exercised through list cleanup paths, but it does not have a
standalone direct rescue check.

Rescue mode does not make an incomplete project valid. It only answers this
practical question:

```text
Which existing functions can still be tested right now?
```

Use diagnose mode to fix project structure. Use the normal tester for final
validation.

## Tester Self-Test

The tester has a self-test suite that creates temporary broken target projects
under `/tmp`:

```sh
make self-test
```

It checks that:

- diagnose fails and explains a missing `libft.h`;
- diagnose detects a partial `libft.a`;
- rescue runs an existing `ft_strlen`;
- rescue skips missing symbols such as `ft_split`;
- the menu falls back safely when there is no interactive terminal.
