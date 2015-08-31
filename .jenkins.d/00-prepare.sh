#!/usr/bin/env bash
set -e
set -x

git rm -qrf .
git clone --depth=1 https://github.com/named-data-ndnSIM/ns-3-dev ns-3
git clone https://github.com/named-data-ndnSIM/pybindgen.git pybindgen
mkdir -p ns-3/src/ndnSIM
mv .git ns-3/src/ndnSIM/
(cd ns-3/src/ndnSIM/ && git reset -q && git checkout . && git submodule update --init)

mv ns-3/src/ndnSIM/.jenkins.d . # move CI scripts back, so the rest of them can be executed
