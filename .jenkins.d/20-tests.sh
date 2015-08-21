#!/usr/bin/env bash
set -x
set -e

JDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source "$JDIR"/util.sh

pushd ns-3 >/dev/null

# Run unit tests
if [[ -n "$XUNIT" ]]; then
    ./waf --run "ndnSIM-unit-tests --log_format=XML --log_sink=build/xunit-report.xml --log_level=all --report_level=no"
else
    ./waf --run "ndnSIM-unit-tests -l test_suite"
fi

popd >/dev/null
