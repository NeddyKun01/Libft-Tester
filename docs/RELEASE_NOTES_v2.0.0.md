# Libft Tester v2.0.0

Release notes for the v2.0.0 Web-first release.

## Highlights

- Repositioned Libft Tester around a Web-first report experience while keeping
  the terminal workflow fast and stable.
- Added `--web` as the preferred standalone Web dashboard output.
- Kept `--html` as a compatibility alias for existing scripts.
- Added named run presets: `quick`, `review`, `school`, `ci`, `web`, `html`,
  and `brutal`.
- Added guided preset selection in the interactive menu.
- Added optional `preset` support to `.libft-tester.json`.
- Added `--compare PATH` for comparing two Libft roots with the same test
  options.
- Upgraded the generated report with a dashboard hero, sticky navigation,
  function search, richer cards, filters, copy commands, and likely-fix hints.
- Added self-tests for preset expansion, config presets, preset listing,
  compare mode, compare-after-preset argument order, and Web dashboard output.
- Removed hand-drawn preview screenshots from the README and replaced them with
  documentation images generated from real tester runs.

## Web Dashboard

```sh
./libft_tester --root ../libft --web --no-color > libft-test-report.html
./libft_tester --root ../libft --preset web > libft-test-report.html
```

The Web dashboard is a single local HTML file. It does not need a server, npm,
React, or external assets. README images for this release are generated from
real command output and real generated HTML, not mock UI screenshots.

## Compare Mode

```sh
./libft_tester --root ../libft --compare ../libft-before --seed 42
./libft_tester --root ../libft --compare ../libft-before --only ft_split --seed 42
```

Compare mode runs both roots with JSON output internally, prints the two scores,
then lists only function-level score differences. It exits successfully only when
both roots pass and their function scores match.
