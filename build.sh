#!/bin/bash
day=${1:-default}

if [ -d "$day" ]; then
    g++ -std=c++20 -g -I./include -Wall -O0 -o "${day}/main" "${day}/main.cpp"
else
    echo "Unable to build day '$day'"
    exit 1
fi
