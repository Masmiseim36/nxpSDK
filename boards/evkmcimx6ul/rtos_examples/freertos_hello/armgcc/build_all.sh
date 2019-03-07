#!/bin/sh
cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Ddr_debug  .
make -j4
cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Ddr_release  .
make -j4
cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug  .
make -j4
cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release  .
make -j4
