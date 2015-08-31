#!/usr/bin/env bash
set -e

JDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source "$JDIR"/util.sh

if has OSX $NODE_LABELS; then
    set -x
    brew update
    brew upgrade
    brew install boost cryptopp pkg-config libxml2
    brew link --force libxml2
    brew cleanup
fi

if has Ubuntu $NODE_LABELS; then
    set -x
    sudo apt-get update -qq -y
    sudo apt-get -qq -y install build-essential
    sudo apt-get -qq -y install libssl-dev libsqlite3-dev libcrypto++-dev

    if has Ubuntu-12.04 $NODE_LABELS; then
        sudo apt-get install -qq -y python-software-properties
        sudo add-apt-repository -y ppa:boost-latest/ppa
        sudo apt-get update -qq -y
        sudo apt-get install -qq -y libboost1.55-all-dev
        sudo apt-get install -qq -y python-gnomedesktop
    else
        sudo apt-get install -qq -y libboost-all-dev
    fi

    sudo apt-get install -qq -y python-setuptools python-dev python-pygraphviz python-kiwi
    sudo apt-get install -qq -y python-pygoocanvas python-gnome2
    sudo apt-get install -qq -y python-rsvg ipython
fi
