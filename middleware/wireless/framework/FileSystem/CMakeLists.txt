set(SOURCES
    fwk_filesystem.c
    # temporary hack until the module is deprecated by FSA
    # do not include fwk_platform_flash.c from platform lib as the content of the file
    # has been moved to fwk_lfs_mflash.c
    # However, to keep current compatibility until FSA is enabled, do as it is part of this module
    ../platform/${CONNFWK_PLATFORM}/fwk_platform_flash.c
)

add_library(connfwk-FileSystem ${SOURCES})

target_include_directories(connfwk-FileSystem
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-FileSystem
    PRIVATE
        connfwk-config
        ${CONNFWK_PLATFORM_LIB}
        ${CONNFWK_BOARD_LIB}
        ${CONNFWK_MCUX_SDK_LIB}
)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-FileSystem-coverage ${SOURCES})
    target_link_libraries(connfwk-FileSystem-coverage PRIVATE connfwk-config)

    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-FileSystem-coverage PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )

    set_target_properties(connfwk-FileSystem-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()
