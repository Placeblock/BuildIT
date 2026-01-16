#!/bin/bash

cmake -DCMAKE_INSTALL_PREFIX:PATH=$PWD/install -DCONFIG_DIR=$PWD/install/config .. && cmake --build . -j 10 && cmake --install . --prefix $PWD/install