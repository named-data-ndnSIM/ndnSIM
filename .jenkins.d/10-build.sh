#!/usr/bin/env bash
set -e

JDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source "$JDIR"/util.sh

set -x

pushd ns-3 >/dev/null

git submodule update --init

# Cleanup
sudo rm -Rf build/ .waf-1* .waf3-1*
find . -name '*.pyc' | sudo xargs rm -f

if has OSX $NODE_LABELS; then
    export PKG_CONFIG_PATH="$PKG_CONFIG_PATH:/usr/local/opt/libxml2/lib/pkgconfig"
    if [[ -n $USE_OPENSSL_1_1 ]]; then
        OPENSSL="--with-openssl=/usr/local/opt/openssl@1.1"
    fi
fi

./waf -j${WAF_JOBS:-1} configure -d debug --enable-examples --enable-tests $OPENSSL
./waf -j${WAF_JOBS:-1} build

# Install
sudo_preserve_env PATH -- ./waf -j${WAF_JOBS:-1} install

if has Linux $NODE_LABELS; then
    sudo ldconfig
elif has FreeBSD $NODE_LABELS; then
    sudo ldconfig -a
fi

popd >/dev/null
