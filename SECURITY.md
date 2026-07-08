# Security Policy

## Supported Versions

Security fixes are provided for the latest released version and the current
`main` branch.

| Version | Supported |
| --- | --- |
| latest release | yes |
| `main` | yes |
| older releases | no |

## Reporting A Vulnerability

Please report suspected security issues privately through GitHub's private
vulnerability reporting mechanism for this repository, if available.

If that channel is not available, contact the maintainer directly through
GitHub. Please do not open a public issue for a private security concern.

Include:

- a clear description of the vulnerability;
- the affected version or commit;
- steps to reproduce the issue;
- relevant logs, proof-of-concept code, or sample inputs;
- the target repository or commit being tested, when relevant.

What to expect:

- acknowledgement within 5 business days;
- triage and an initial assessment;
- coordination of a fix and release if the report is accepted;
- updates until the issue is resolved or closed.

Please do not disclose the vulnerability publicly until a fix has been released
or the report has been closed.

## Scope

This project is a local and CI test harness. Security concerns relevant to this
repository include:

- unsafe handling of repository paths or test inputs;
- command execution or subprocess behavior;
- issues affecting build, test, or report generation;
- CI configuration problems that could expose sensitive data.

Issues in a user's target implementation are usually not vulnerabilities in this
tester unless the tester handles them unsafely.
