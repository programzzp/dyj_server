#!/bin/bash

sudo apt-get install gcc
sudo apt-get install cmake
sudo apt-get install openssl

rm -rf build/*

cd build

cmake ..

make

cd ..


