#!/usr/bin/env bash
set -x
set -e

cd /tmp
BUILD="no"
if [ ! -d ndn-cxx ]; then
    git clone git://github.com/named-data/ndn-cxx
    cd ndn-cxx
    BUILD="yes"
else
    cd ndn-cxx
    INSTALLED_VERSION=`git rev-parse HEAD || echo NONE`
    sudo rm -Rf latest-version
    git clone git://github.com/named-data/ndn-cxx latest-version
    cd latest-version
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

IS_UBUNTU_12_04=$( python -c "print 'yes' if 'Ubuntu-12.04' in '$NODE_LABELS'.strip().split(' ') else 'no'" )
if [[ $IS_UBUNTU_12_04 == "yes" ]]; then
    EXTRA_FLAGS=" --boost-libs=/usr/lib/x86_64-linux-gnu"
fi

./waf configure -j1 --color=yes --without-osx-keychain $EXTRA_FLAGS
./waf -j1 --color=yes
sudo ./waf install -j1 --color=yes
