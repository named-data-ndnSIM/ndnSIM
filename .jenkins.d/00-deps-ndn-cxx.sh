#!/usr/bin/env bash
set -x
set -e

cd /tmp
BUILD="no"
if [ ! -d ndn-cxx ]; then
    git clone git://github.com/named-data/ndn-cxx
    cd ndn-cxx
    # TEMPORARY, the following must be removed after issue if fixed
    git checkout 81a6c5dea60cea97c60dab0d78576c0d3b4e29ed
    BUILD="yes"
else
    cd ndn-cxx
    INSTALLED_VERSION=`git rev-parse HEAD || echo NONE`
    sudo rm -Rf latest-version
    git clone git://github.com/named-data/ndn-cxx latest-version
    cd latest-version
    # TEMPORARY, the following must be removed after issue if fixed
    git checkout 81a6c5dea60cea97c60dab0d78576c0d3b4e29ed
    LATEST_VERSION=`git rev-parse HEAD || echo UNKNOWN`
    cd ..
    rm -Rf latest-version
    if [ "$INSTALLED_VERSION" != "$LATEST_VERSION" ]; then
        cd ..
        sudo rm -Rf ndn-cxx
        git clone --depth 1 git://github.com/named-data/ndn-cxx
        cd ndn-cxx
        BUILD="yes"
    fi
fi

sudo rm -Rf /usr/local/include/ndn-cxx
sudo rm -f /usr/local/lib/libndn-cxx*
sudo rm -f /usr/local/lib/pkgconfig/libndn-cxx*

if [ "$BUILD" = "yes" ]; then
    sudo ./waf distclean -j1 --color=yes
fi

./waf configure -j1 --color=yes --without-osx-keychain
./waf -j1 --color=yes
sudo ./waf install -j1 --color=yes
