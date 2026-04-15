#!/usr/bin/env bash
if [ $(uname -o) == "Darwin" ]; then
    docker build --platform=linux/amd64 -t dolphian .  
    docker run -v `pwd`/../mysyslog/:/tmp -v `pwd`:/root -it --rm --platform linux/amd64 dolphian /bin/bash
elif [ $(uname -o) == "GNU/Linux" ]; then
    echo "[BUILD] begin..."
    make -C /tmp/. deb-all
    make
    echo "[BUILD] done."
fi
