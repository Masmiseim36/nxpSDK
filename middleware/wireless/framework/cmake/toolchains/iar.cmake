if(WIN32)
    set(EXE ".exe")
endif()

message(STATUS "Setting TOOLCHAIN_DIR to ICCARM_DIR...")
set(TOOLCHAIN_DIR $ENV{ICCARM_DIR})
string(REGEX REPLACE "\\\\" "/" TOOLCHAIN_DIR "${TOOLCHAIN_DIR}")

if(NOT TOOLCHAIN_DIR)
    message(STATUS "***ICCARM_DIR is not set, assume toolchain bins are in your PATH***")
    set(TOOLCHAIN_BIN_DIR "")
else()
    message(STATUS "TOOLCHAIN_DIR: " ${TOOLCHAIN_DIR})
    set(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_DIR}/bin/)
endif()

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_EXECUTABLE_SUFFIX_CXX ${CMAKE_EXECUTABLE_SUFFIX})

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_DIR}iccarm${EXE})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_DIR}iccarm${EXE})
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_DIR}iasmarm${EXE})
set(CMAKE_AR ${TOOLCHAIN_BIN_DIR}iarchive)
set(CMAKE_LINKER ${TOOLCHAIN_BIN_DIR}ilinkarm)

set(CMAKE_STATIC_LIBRARY_PREFIX_C "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX_C ".a")

message(STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})

set(COMMON_C_FLAGS  "-e --diag_suppress Pa082,Pa050,Pe014")

set(CMAKE_C_FLAGS_INIT             "${COMMON_C_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT           "${COMMON_C_FLAGS} --c++")
set(CMAKE_ASM_FLAGS_INIT           "-s+ -M{} -w- -j")
set(CMAKE_EXE_LINKER_FLAGS_INIT    "--diag_suppress Li065 --entry Reset_Handler --vfe --semihosting --redirect _Printf=_PrintfSmallNoMb --redirect _Scanf=_ScanfSmallNoMb --no_out_extension --text_out locale")

# Optimize for debug operation
set(CMAKE_C_FLAGS_DEBUG            "-r -On" CACHE STRING "C compiler flags for Debug build type")
set(CMAKE_CXX_FLAGS_DEBUG          "-r -On" CACHE STRING "C++ compiler flags for Debug build type")
set(CMAKE_ASM_FLAGS_DEBUG          "-r" CACHE STRING "ASM compiler flags for Debug build type")

# Optimization for speed
set(CMAKE_C_FLAGS_RELEASE          "-Oh -DNDEBUG" CACHE STRING "C compiler flags for Release build type")
set(CMAKE_CXX_FLAGS_RELEASE        "-Oh -DNDEBUG" CACHE STRING "C++ compiler flags for Release build type")
set(CMAKE_ASM_FLAGS_RELEASE        "-DNDEBUG" CACHE STRING "ASM compiler flags for Release build type")

# Optimization for size
set(CMAKE_C_FLAGS_MINSIZEREL       "-Ohz -DNDEBUG" CACHE STRING "C compiler flags for MinSizeRel build type")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Ohz -DNDEBUG" CACHE STRING "C++ compiler flags for MinSizeRel build type")
set(CMAKE_ASM_FLAGS_MINSIZEREL     "-DNDEBUG" CACHE STRING "ASM compiler flags for MinSizeRel build type")

# Balanced optimization with debug infos
set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-r -Oh -DNDEBUG" CACHE STRING "C compiler flags for RelWithDebInfo build type")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-r -Oh -DNDEBUG" CACHE STRING "C++ compiler flags for RelWithDebInfo build type")
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO "-r -DNDEBUG" CACHE STRING "ASM compiler flags for RelWithDebInfo build type")

SET(CMAKE_CXX_FLAGS_COVERAGE
    "${CMAKE_CXX_FLAGS_DEBUG} ${COVERAGE_C_FLAGS}" CACHE STRING
    "Flags used by the C++ compiler during coverage builds."
    FORCE )
SET(CMAKE_C_FLAGS_COVERAGE
    "${CMAKE_C_FLAGS_DEBUG} ${COVERAGE_C_FLAGS}" CACHE STRING
    "Flags used by the C compiler during coverage builds."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${COVERAGE_L_FLAGS}" CACHE STRING
    "Flags used for linking binaries during coverage builds."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${COVERAGE_L_FLAGS}" CACHE STRING
    "Flags used by the shared libraries linker during coverage builds."
    FORCE )
SET( CMAKE_STATIC_LINKER_FLAGS_COVERAGE
    "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} ${COVERAGE_L_FLAGS}" CACHE STRING
    "Flags used by the static libraries linker during coverage builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    CMAKE_STATIC_LINKER_FLAGS_COVERAGE
    COMPILE_DEFINITIONS_COVERAGE
)

# -------------------------------------------------------------------------- #
#                                    Flags                                   #
# -------------------------------------------------------------------------- #

# Set FreeRTOS toolchain port folder name
set(FREERTOS_TOOLCHAIN IAR)

# -------------------------------------------------------------------------- #
#                                   Helpers                                  #
# -------------------------------------------------------------------------- #

# Set linker script used by the linker
function(connfwk_target_set_linker_script target keyword path)
    target_link_options(${target} ${keyword}
        LINKER:--config,${path}
    )
endfunction()

# Enables map file generation
function(connfwk_target_generate_map_file target keyword path)
    target_link_options(${target} ${keyword}
        LINKER:--map,${path}
    )
endfunction()

# Set linker define
function(connfwk_target_set_link_symbol target keyword define)
    target_link_options(${target} ${keyword}
        LINKER:--config_def,${define}
    )
endfunction()

function(connfwk_export_target_to_bin target)
    get_target_property(type ${target} TYPE)
    if(type MATCHES "EXECUTABLE")
        # message(STATUS "Target ${target} will be exported to raw binary format")
        set(target_bin_filename ${target}.bin)
        add_custom_command(
            OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_bin_filename}
            COMMAND ${CMAKE_IAR_ELFTOOL} ARGS --bin ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}${CMAKE_EXECUTABLE_SUFFIX_C} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_bin_filename}
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
            COMMAND ${CMAKE_IAR_ELFTOOL} ARGS --srec ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}${CMAKE_EXECUTABLE_SUFFIX_C} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_srec_filename}
            DEPENDS ${target}
        )
        add_custom_target(export_${target}_to_srec ALL
            DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target_srec_filename}
        )
    endif()
endfunction()
