#!/usr/bin/env bash
set -x
set -e

# git submodule init
# git submodule sync
# git submodule update

# COVERAGE=$( python -c "print '--with-coverage' if 'code-coverage' in '$JOB_NAME' else ''" )
IS_UBUNTU_12_04=$( python -c "print 'yes' if 'Ubuntu-12.04' in '$NODE_LABELS'.strip().split(' ') else 'no'" )
if [[ $IS_UBUNTU_12_04 == "yes" ]]; then
    EXTRA_FLAGS=" --boost-libs=/usr/lib/x86_64-linux-gnu"
fi

# Cleanup
sudo ./waf -j1 distclean

# Configure/build in debug mode
./waf -j1 configure --enable-examples --enable-tests $EXTRA_FLAGS
./waf -j1 build

# # Cleanup
# sudo ./waf -j1 --color=yes distclean

# # Configure/build in optimized mode without tests with precompiled headers
# ./waf -j1 --color=yes configure
# ./waf -j1 --color=yes build

# # Cleanup
# sudo ./waf -j1 --color=yes distclean

# # Configure/build in optimized mode
# ./waf -j1 --color=yes configure --with-tests --without-pch $COVERAGE
# ./waf -j1 --color=yes build

# # (tests will be run against optimized version)

# # Install
# sudo ./waf -j1 --color=yes install
