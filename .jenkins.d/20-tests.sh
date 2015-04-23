#!/usr/bin/env bash
set -x
set -e

JDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source "$JDIR"/util.sh

# Run unit tests
if [[ -n "$XUNIT" ]]; then
    ./build/src/ndnSIM/tests/ns3-dev-ndnSIM-unit-tests-debug --log_format=XML --log_sink=build/xunit-report.xml --log_level=all --report_level=no
else
    ./build/src/ndnSIM/tests/ns3-dev-ndnSIM-unit-tests-debug -l test_suite
fi
