# Copyright (c) 2022-2023, NXP.
# All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the copyright holder nor the
#    names of its contributors may be used to endorse or promote products
#    derived from this software without specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# TOOLCHAIN EXTENSION
if(WIN32)
    set(TOOLCHAIN_EXT ".exe")
else()
    set(TOOLCHAIN_EXT "")
endif()

# TOOLCHAIN_DIR AND NANO LIBRARY
set(TOOLCHAIN_DIR $ENV{ARMGCC_DIR})
string(REGEX REPLACE "\\\\" "/" TOOLCHAIN_DIR "${TOOLCHAIN_DIR}")

if(NOT TOOLCHAIN_DIR)
    message(STATUS "***ARMGCC_DIR is not set, assume toolchain bins are in your PATH***")
    set(TOOLCHAIN_BIN_DIR "")
else()
    message(STATUS "TOOLCHAIN_DIR: " ${TOOLCHAIN_DIR})
    set(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_DIR}/bin/)
endif()

# try_compile will test the compiler with a library target instead of an executable
# It is required here to avoid linking issue when cross-compiling
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# All executables will have the .elf suffix and libs will have .a
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_EXECUTABLE_SUFFIX_CXX ${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_PREFIX "arm-none-eabi")

set(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_DIR}${TOOLCHAIN_PREFIX}-gcc${TOOLCHAIN_EXT})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_DIR}${TOOLCHAIN_PREFIX}-g++${TOOLCHAIN_EXT})
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_DIR}${TOOLCHAIN_PREFIX}-gcc${TOOLCHAIN_EXT})
set(CMAKE_OBJCOPY ${TOOLCHAIN_BIN_DIR}${TOOLCHAIN_PREFIX}-objcopy${TOOLCHAIN_EXT} CACHE INTERNAL "objcopy tool")
set(CMAKE_OBJDUMP ${TOOLCHAIN_BIN_DIR}${TOOLCHAIN_PREFIX}-objdump${TOOLCHAIN_EXT} CACHE INTERNAL "objdump tool")

message(STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})

set(COMMON_C_FLAGS  "-Wall -Wno-expansion-to-defined -Wno-endif-labels -fmessage-length=0 -fdata-sections -ffunction-sections")

set(CMAKE_C_FLAGS_INIT             "${COMMON_C_FLAGS} -std=gnu99")
set(CMAKE_CXX_FLAGS_INIT           "${COMMON_C_FLAGS} -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS_INIT           "${COMMON_C_FLAGS} -x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS_INIT    "${COMMON_C_FLAGS} -specs=nano.specs -specs=nosys.specs -Xlinker --gc-sections")

# Optimize for debug operation
set(CMAKE_C_FLAGS_DEBUG             "-g -O0" CACHE STRING "C compiler flags for Debug build type")
set(CMAKE_CXX_FLAGS_DEBUG           "-g -O0" CACHE STRING "C++ compiler flags for Debug build type")
set(CMAKE_ASM_FLAGS_DEBUG           "-g -O0" CACHE STRING "ASM compiler flags for Debug build type")

# Optimization for speed
set(CMAKE_C_FLAGS_RELEASE          "-O3 -DNDEBUG" CACHE STRING "C compiler flags for Release build type")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG" CACHE STRING "C++ compiler flags for Release build type")
set(CMAKE_ASM_FLAGS_RELEASE        "-O3 -DNDEBUG" CACHE STRING "ASM compiler flags for Release build type")

# Optimization for size
set(CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG" CACHE STRING "C compiler flags for MinSizeRel build type")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG" CACHE STRING "C++ compiler flags for MinSizeRel build type")
set(CMAKE_ASM_FLAGS_MINSIZEREL     "-Os -DNDEBUG" CACHE STRING "ASM compiler flags for MinSizeRel build type")

# Balanced optimization with debug infos
set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-g -O2 -DNDEBUG" CACHE STRING "C compiler flags for RelWithDebInfo build type")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 -DNDEBUG" CACHE STRING "C++ compiler flags for RelWithDebInfo build type")
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO "-g -O2 -DNDEBUG" CACHE STRING "ASM compiler flags for RelWithDebInfo build type")

# -------------------------------------------------------------------------- #
#                                    Flags                                   #
# -------------------------------------------------------------------------- #

# Set FreeRTOS toolchain port folder name
set(FREERTOS_TOOLCHAIN GCC)

# -------------------------------------------------------------------------- #
#                                   Helpers                                  #
# -------------------------------------------------------------------------- #

# Set linker script used by the linker
function(connfwk_target_set_linker_script target keyword path)
    target_link_options(${target} ${keyword}
        LINKER:-T,${path}
    )
endfunction()

# Enables map file generation
function(connfwk_target_generate_map_file target keyword path)
    target_link_options(${target} ${keyword}
        LINKER:-Map,${path}
        LINKER:-print-memory-usage
    )
endfunction()

# Set linker define
function(connfwk_target_set_link_symbol target keyword define)
    target_link_options(${target} ${keyword}
        LINKER:--defsym,${define}
    )
endfunction()

function(connfwk_export_target_to_bin target)
    get_target_property(type ${target} TYPE)
    if(type MATCHES "EXECUTABLE")
        # message(STATUS "Target ${target} will be exported to raw binary format")
        set(target_bin_filename ${target}.bin)
        add_custom_command(
            OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_bin_filename}
            COMMAND ${CMAKE_OBJCOPY} ARGS -v -O binary ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}${CMAKE_EXECUTABLE_SUFFIX_C} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_bin_filename}
            DEPENDS ${target}
        )
        add_custom_target(export_${target}_to_bin ALL
            DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_bin_filename}
        )
    endif()
endfunction()

function(connfwk_export_target_to_srec target)
    get_target_property(type ${target} TYPE)
    if(type MATCHES "EXECUTABLE")
        # message(STATUS "Target ${target} will be exported to srec format")
        set(target_srec_filename ${target}.srec)
        add_custom_command(
            OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_srec_filename}
            COMMAND ${CMAKE_OBJCOPY} ARGS -v -O srec ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}${CMAKE_EXECUTABLE_SUFFIX_C} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_srec_filename}
            DEPENDS ${target}
        )
        add_custom_target(export_${target}_to_srec ALL
            DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_srec_filename}
        )
    endif()
endfunction()
