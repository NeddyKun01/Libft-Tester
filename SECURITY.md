# Security Policy

## Supported Versions

This repository is a C++ tester for 42 libft projects. Security fixes are only provided for the latest supported release line.

| Version | Supported |
| ------- | --------- |
| 1.0.x   | :white_check_mark: |
| < 1.0.0 | :x:               |

## Reporting a Vulnerability

Please report suspected security issues privately through GitHub's private vulnerability reporting mechanism for this repository, if available. If that channel is not available, contact the maintainer directly through GitHub and do not open a public issue.

Please include the following information in your report:

- a clear description of the vulnerability;
- the affected version(s);
- steps to reproduce the issue;
- any relevant logs, proof-of-concept code, or sample inputs;
- the target repository or commit being tested, when relevant.

What to expect:

- Acknowledgement within 5 business days;
- An assessment of the report and triage status;
- Coordination of a fix and release if the issue is accepted;
- Updates on remediation progress until the issue is resolved.

Please do not disclose the vulnerability publicly until a fix has been released or the report has been closed.

## Scope

This project is primarily a local and CI test harness. Security concerns relevant to this repository include:

- unsafe handling of repository paths or test inputs;
- command execution or subprocess behavior in the test runner;
- issues affecting the build, test, or reporting workflow;
- problems in the CI configuration that could expose sensitive data.
