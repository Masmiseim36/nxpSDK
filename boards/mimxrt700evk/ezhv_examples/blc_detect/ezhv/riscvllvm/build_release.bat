if exist CMakeFiles (RD /s /Q CMakeFiles)
if exist Makefile (DEL /s /Q /F Makefile)
if exist build.ninja (DEL /s /Q /F build.ninja)
if exist cmake_install.cmake (DEL /s /Q /F cmake_install.cmake)
if exist CMakeCache.txt (DEL /s /Q /F CMakeCache.txt)
cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../../tools/cmake_toolchain_files/riscvllvm.cmake" -G "Ninja" -DCMAKE_BUILD_TYPE=release  .
ninja -j4 2> build_log.txt 
