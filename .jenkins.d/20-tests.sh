#!/usr/bin/env bash
set -x

JDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source "$JDIR"/util.sh

set -e

pushd ns-3 >/dev/null

BOOST_VERSION=$(python -c "import sys; sys.path.append('build/c4che'); import _cache; v = _cache.BOOST_VERSION.split('_'); print int(v[0]) * 100000 + int(v[1]) * 100;")

ut_log_args() {
    if (( BOOST_VERSION >= 106200 )); then
        echo --logger=HRF,test_suite,stdout:XML,all,build/xunit-${1:-report}.xml
    else
        if [[ -n $XUNIT ]]; then
            echo --log_level=all $( (( BOOST_VERSION >= 106000 )) && echo -- ) \
                 --log_format2=XML --log_sink2=build/xunit-${1:-report}.xml
        else
            echo --log_level=test_suite
        fi
    fi
}

ASAN_OPTIONS="color=always"
ASAN_OPTIONS+=":detect_leaks=false"
ASAN_OPTIONS+=":detect_stack_use_after_return=true"
ASAN_OPTIONS+=":check_initialization_order=true"
ASAN_OPTIONS+=":strict_init_order=true"
ASAN_OPTIONS+=":detect_invalid_pointer_pairs=1"
ASAN_OPTIONS+=":detect_container_overflow=false"
ASAN_OPTIONS+=":strict_string_checks=true"
ASAN_OPTIONS+=":strip_path_prefix=${PWD}/"
export ASAN_OPTIONS

# Run unit tests
./waf --run "ndnSIM-unit-tests $(ut_log_args)"

popd >/dev/null
