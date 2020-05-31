#!/usr/bin/env bash
set -e

JDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source "$JDIR"/util.sh

set -x

pushd ns-3 >/dev/null

export BOOST_TEST_BUILD_INFO=1
export BOOST_TEST_COLOR_OUTPUT=1
export BOOST_TEST_DETECT_MEMORY_LEAK=0
export BOOST_TEST_LOGGER=HRF,test_suite,stdout:XML,all,build/xunit-log.xml

# Run unit tests
./waf --run "ndnSIM-unit-tests"

popd >/dev/null
