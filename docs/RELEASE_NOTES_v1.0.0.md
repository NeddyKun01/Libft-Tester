# Libft Tester v1.0.0

First stable release of Libft Tester.

## Highlights

- Full current 42 Libft function coverage.
- Compact terminal output with `OK`, `NOK`, `MOK`, and `MNOK`.
- Fork-isolated suite execution with crash and timeout reporting.
- Malloc failure simulation for allocation-heavy functions.
- Reproducible seeded pseudo-random checks.
- Run profiles: `quick`, `normal`, `strict`, and `brutal`.
- JSON reports for scripts and CI.
- Standalone HTML reports for visual inspection.
- Valgrind leak-checking target.
- Per-function coverage metadata and explanations.
- Beginner-friendly debugging hints.
- GitHub Actions workflow for testing an external Libft repository.

## Quick Start

```sh
git clone git@github.com:OWNER/Libft-Tester.git
cd Libft-Tester
make ROOT_DIR=../libft
```

## Useful Commands

```sh
make ROOT_DIR=../libft summary
make ROOT_DIR=../libft strict
make ROOT_DIR=../libft report
make ROOT_DIR=../libft report-html
make ROOT_DIR=../libft leaks ARGS="--only ft_split --no-color"
```

## Documentation

- `README.md`
- `docs/USAGE.md`
- `docs/TROUBLESHOOTING.md`
- `docs/COVERAGE.md`
- `docs/CONTRIBUTING_TESTS.md`

