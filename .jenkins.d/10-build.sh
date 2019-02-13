#!/usr/bin/env bash
set -x
set -e

JDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source "$JDIR"/util.sh

pushd ns-3 >/dev/null

git submodule update --init

# Cleanup
sudo rm -Rf build/ .waf-1* .waf3-1*
find . -name '*.pyc' | sudo xargs rm -f

# Configure/build in debug mode
if has OSX $NODE_LABELS; then
    export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:/usr/local/opt/libxml2/lib/pkgconfig"
fi
./waf -j${WAF_JOBS:-1} configure -d debug --enable-examples --enable-tests $EXTRA_FLAGS
./waf -j${WAF_JOBS:-1} build

# Install
sudo ./waf -j${WAF_JOBS:-1} install

if has Linux $NODE_LABELS; then
    sudo ldconfig
elif has FreeBSD $NODE_LABELS; then
    sudo ldconfig -a
fi

popd >/dev/null
