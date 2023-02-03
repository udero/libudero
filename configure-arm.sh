#!/bin/bash

#cd build-arm
cmake ../ -DCMAKE_C_COMPILER="arm-linux-gnueabi-gcc" -DCMAKE_CXX_COMPILER="arm-linux-gnueabi-g++" -DCMAKE_AR="/usr/bin/arm-linux-gnueabi-ar" -DCMAKE_POSITION_INDEPENDENT_CODE=ON