#!/usr/bin/env bash
set -e

JDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source "$JDIR"/util.sh

set -x

if has OSX $NODE_LABELS; then
    brew update
    brew upgrade
    brew install boost pkg-config cryptopp openssl
    brew install libxml2
    brew link --force libxml2
    brew cleanup
fi

if has Ubuntu $NODE_LABELS; then
    sudo apt-get -qq update
    sudo apt-get -qq install build-essential pkg-config libboost-all-dev \
                             libcrypto++-dev libsqlite3-dev libssl-dev
    sudo apt-get install -qq -y python-setuptools python-dev python-pygraphviz python-kiwi
    # sudo apt-get install -qq -y python-pygoocanvas python-gnome2
    # sudo apt-get install -qq -y python-rsvg ipython
fi
