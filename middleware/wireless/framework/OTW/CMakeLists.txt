
list(APPEND INCLUDES
    Interface
)

#Check if the k32w0 transceiver is enabled
string(FIND ${CONNFWK_TRANSCEIVER} "k32w0" outString)
if ( ${outString} GREATER_EQUAL 0)
    list(APPEND SOURCES
        k32w0_transceiver/fwk_otw.c
    )
else()
    message(FATAL_ERROR "No implementation for OTW")
endif()

add_library(connfwk-OTW ${SOURCES})

# Making those includes PUBLIC will share them to the other targets linking this lib
target_include_directories(connfwk-OTW PUBLIC ${INCLUDES})

target_compile_definitions(connfwk-OTW PRIVATE ${COMPILE_PRIVATE_DEFINITIONS})

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-OTW
    PRIVATE
        connfwk-config
        ${CONNFWK_MCUX_SDK_LIB}
)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-OTW-coverage ${SOURCES})
    target_link_libraries(connfwk-OTW-coverage PRIVATE connfwk-config)

    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-OTW-coverage PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )

    set_target_properties(connfwk-OTW-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()
