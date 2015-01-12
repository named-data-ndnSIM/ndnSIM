NFD - Named Data Networking Forwarding Daemon
=============================================

Files in this directory are imported from NFD (Named Data Networking Forwarding Daemon)
project (http://named-data.net/doc/NFD/).

The codebase can be merged with the upstream with the following command:

    git subtree pull --prefix=NFD https://github.com/named-data/NFD master

There are several ndnSIM-specific changes to NFD codebase and manual conflict resolution
may be required.

Changes in NFD codebase
-----------------------

The following commits introduced changes to NFD codebase:

- `e98f2e8` NFD: Removing parts irrelevant for the simulator
- `7245d5d` NFD: Adding readme and auto-generated headers
- `dc88971` NFD: Disabling all "standard" face types in Face manager
- `0fb253e` NFD: Enable metric parameter on a face
- `1de5da6` NFD: Redirecting nfd::scheduler to ns3::Simulator and getting rid of GlobalIo
- `dbc7794` NFD: Replace NFD logger with NS3 logging facility
- `de1f773` NFD: Keep both the CS of NFD and ndnSIM
- `542a623` NFD: Extending ConfigFile to support passing of a parsed ConfigSection instead of
  raw config file
- `9bcf39e` NFD: Extend forwarding pipelines with beforeSatisfyInterest and
  beforeExpirePendingInterest signals
