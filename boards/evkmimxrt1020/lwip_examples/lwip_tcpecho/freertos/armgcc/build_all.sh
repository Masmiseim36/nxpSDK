#!/bin/sh
cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../../tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=flexspi_nor_sdram_debug  .
make -j4
cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../../tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=flexspi_nor_sdram_release  .
make -j4
