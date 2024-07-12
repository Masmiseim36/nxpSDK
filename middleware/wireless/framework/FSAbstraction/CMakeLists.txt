string(TOUPPER ${CONNFWK_FSABSTRACTION} CONNFWK_FSABSTRACTION)

if(CONNFWK_FSABSTRACTION MATCHES "LFS")
    set(SOURCES
        fwk_fs_abstraction.c
        fwk_lfs_mflash.c
    )
elseif(CONNFWK_FSABSTRACTION MATCHES "NVS")
    set(SOURCES
        fwk_nvsfs_abstraction.c
    )
else()
    message( SEND_ERROR "CONNFWK_FSABSTRACTION ${CONNFWK_FSABSTRACTION} value not supported!")
endif()


add_library(connfwk-FSAbstraction ${SOURCES})

target_include_directories(connfwk-FSAbstraction
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)



# Get common configs from the connfwk-config interface
if(CONNFWK_FSABSTRACTION MATCHES "LFS")
    target_link_libraries(connfwk-FSAbstraction
        PRIVATE
            connfwk-config
            ${CONNFWK_PLATFORM_LIB}
            ${CONNFWK_BOARD_LIB}
        PUBLIC
            ${CONNFWK_MCUX_SDK_LIB}
    )
else()
    target_link_libraries(connfwk-FSAbstraction
    PRIVATE
        connfwk-config
        connfwk-NVS
        ${CONNFWK_PLATFORM_LIB}
        ${CONNFWK_BOARD_LIB}
    PUBLIC
        ${CONNFWK_MCUX_SDK_LIB}
    )
endif()

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-FSAbstraction-coverage ${SOURCES})
    target_link_libraries(connfwk-FSAbstraction-coverage PRIVATE connfwk-config)

    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-FSAbstraction-coverage PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )

    set_target_properties(connfwk-FSAbstraction-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()
