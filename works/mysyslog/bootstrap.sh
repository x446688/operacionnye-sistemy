#!/usr/bin/env bash
if [ $(uname -o) == "Darwin" ]; then
    docker build --platform=linux/amd64 -t dolphian .  
    docker run -it --rm --platform linux/amd64 dolphian /bin/bash
elif [ $(uname -o) == "GNU/Linux" ]; then
    make
    make run
fi
