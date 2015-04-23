#!/usr/bin/env bash
set -x
set -e

JDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source "$JDIR"/util.sh

git submodule update --init

# Cleanup
sudo ./waf -j1 distclean

if has Ubuntu-12.04 $NODE_LABELS; then
    EXTRA_FLAGS=" --boost-libs=/usr/lib/x86_64-linux-gnu"
fi

# Configure/build in debug mode
./waf -j1 configure -d debug --enable-modules=ndnSIM --enable-examples --enable-tests $EXTRA_FLAGS
./waf -j1 build

# Install
sudo ./waf -j1 install

if has Linux $NODE_LABELS; then
    sudo ldconfig
elif has FreeBSD $NODE_LABELS; then
    sudo ldconfig -a
fi
