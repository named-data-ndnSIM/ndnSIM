#!/usr/bin/env bash
set -x
set -e

IS_OSX=$( python -c "print 'yes' if 'OSX' in '$NODE_LABELS'.strip().split(' ') else 'no'" )
IS_UBUNTU=$( python -c "print 'yes' if 'Ubuntu' in '$NODE_LABELS'.strip().split(' ') else 'no'" )

if [[ $IS_OSX == "yes" ]]; then
    brew update
    brew upgrade
    brew install boost cryptopp pkg-config libxml2
    brew link --force libxml2
    brew cleanup
fi

if [[ $IS_UBUNTU == "yes" ]]; then
    sudo apt-get update -q -y
    sudo apt-get -y install build-essential
    sudo apt-get -y install libssl-dev libsqlite3-dev libcrypto++-dev

    IS_12_04=$( python -c "print 'yes' if 'Ubuntu-12.04' in '$NODE_LABELS'.strip().split(' ') else 'no'" )
    if [[ $IS_12_04 == "yes" ]]; then
        sudo apt-get install -y python-software-properties
        sudo add-apt-repository -y ppa:boost-latest/ppa
        sudo apt-get update -q -y
        sudo apt-get install -y libboost1.55-all-dev
        sudo apt-get install -y python-gnomedesktop
    else
        sudo apt-get install -y libboost-all-dev
    fi

    sudo apt-get install -y python-dev python-pygraphviz python-kiwi
    sudo apt-get install -y python-pygoocanvas python-gnome2
    sudo apt-get install -y python-rsvg ipython
fi
