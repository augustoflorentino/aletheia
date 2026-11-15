# Security policy

## Supported versions

Only the latest release receives fixes.

| Version | Supported |
|---------|-----------|
| 0.9.x   | yes       |
| < 0.9   | no        |

## Reporting a vulnerability

Report it privately through GitHub: **Security → Report a vulnerability** on
this repository. Do not open a public issue.

Include the version (`aletheia --version`), the command line or the API calls
that trigger the problem, and the input file if there is one. You should get
an answer within a week.

## Scope

Aletheia is a simulator. The attacks it demonstrates on the simulated machine
(fault injection, cold boot, DMA, power analysis) are features, not
vulnerabilities. What counts here is a flaw in the host program or in
`libaletheia` itself: memory corruption or a crash from a crafted assembly
file, snapshot file, command line or API call.
