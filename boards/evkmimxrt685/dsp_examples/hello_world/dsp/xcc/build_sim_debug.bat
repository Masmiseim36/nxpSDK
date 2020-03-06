cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../../tools/cmake_toolchain_files/xcc.cmake" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=sim_debug  .
mingw32-make -j4
IF "%1" == "" ( pause ) 
