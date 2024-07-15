#!/bin/sh

mkdir -p ./build/
g++ -Wall -O -o ./build/buildit ./src/simulation/node.cpp ./src/simulation/gate.cpp ./src/buildit.cpp