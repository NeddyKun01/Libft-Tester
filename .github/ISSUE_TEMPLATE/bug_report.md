---
name: Bug report
about: Report a problem in Libft Tester
title: "[Bug]: "
labels: bug
assignees: ""
---

## Summary

Describe the problem clearly.

## Area Affected

Choose the closest area:

- menu
- CLI
- `--review`
- presets
- compare mode
- Web report
- JSON report
- config file
- tests
- CI
- docs
- other

## Steps To Reproduce

1. Set up the tester and target library.
2. Run the command or menu option that fails.
3. Paste the relevant output below.

Example commands:

```sh
make build
./libft_tester --root ../libft --only ft_split --profile strict
./libft_tester --root ../libft --preset review
./libft_tester --root ../libft --compare ../libft-before --seed 42
./libft_tester --root ../libft --web --no-color
```

## Expected Behavior

What did you expect to happen?

## Actual Behavior

What happened instead?

## Environment

- OS:
- Shell:
- Compiler and version:
- Make version:
- Tester version or commit:
- Target repository or commit:
- Command, flags, or menu option used:
- Config file used, if any:

## Output

Paste relevant terminal output, CI logs, Valgrind output, screenshots, or report
snippets.

```text
paste output here
```

## Additional Context

Mention affected functions, suites, reports, or anything else that may help
reproduce the issue.
