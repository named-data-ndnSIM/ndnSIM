#!/usr/bin/env bash
set -e

JDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source "$JDIR"/util.sh

set -x

if has OSX $NODE_LABELS; then
    FORMULAE=(boost pkg-config libxml2)
    if [[ -n $USE_OPENSSL_1_1 ]]; then
        FORMULAE+=(openssl@1.1)
    else
        FORMULAE+=(openssl)
    fi

    brew update
    if [[ -n $TRAVIS ]]; then
        # Travis images come with a large number of brew packages
        # pre-installed, don't waste time upgrading all of them
        for FORMULA in "${FORMULAE[@]}"; do
            brew outdated $FORMULA || brew upgrade $FORMULA
        done
    else
        brew upgrade
    fi
    brew install "${FORMULAE[@]}"
    brew cleanup
fi

if has Ubuntu $NODE_LABELS; then
    sudo apt-get -qq update
    sudo apt-get -qy install build-essential pkg-config libboost-all-dev \
                             libsqlite3-dev libssl-dev

    if [[ $JOB_NAME == *"code-coverage" ]]; then
        sudo apt-get -qy install gcovr lcov libgd-perl
    fi
fi

if has Ubuntu-16.04 $NODE_LABELS; then
    sudo apt-get -qy install python-setuptools python-dev python-pygraphviz castxml \
                             python-kiwi python-gnome2 ipython libcairo2-dev python3-gi \
                             libgirepository1.0-dev python-gi python-gi-cairo \
                             gir1.2-gtk-3.0 gir1.2-goocanvas-2.0 python-pip
    sudo pip install pygraphviz pycairo PyGObject pygccxml
elif has Ubuntu $NODE_LABELS; then
    sudo apt-get -qy install build-essential libsqlite3-dev libboost-all-dev libssl-dev \
                             git python3-setuptools castxml \
                             gir1.2-goocanvas-2.0 gir1.2-gtk-3.0 libgirepository1.0-dev \
                             python3-dev python3-gi python3-gi-cairo \
                             python3-pip python3-pygraphviz python3-pygccxml
    sudo pip3 install kiwi
fi
