#!/bin/sh

mkdir -p ./build/
g++ -Wall -O -o ./build/buildit ./src/tree.cpp ./src/buildit.cpp