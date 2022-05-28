#!/usr/bin/env bash

set -e 

sudo apt-get update
sudo apt-get install -y  libtool libgtest-dev cmake
sudo apt-get install -y libboost-all-dev
sudo apt-get install -y libjsoncpp-dev
sudo apt-get install -y  bison flex
sudo apt-get install -y libgtest-dev
sudo apt-get install -y doxygen
sudo apt-get install -y gcovr



cd /usr/src/googletest
sudo cmake .
sudo cmake --build . --target install
