set(SOURCES
    Source/OtaSupport.c
    Source/OtaExternalFlash.c
    Source/OtaInternalFlash.c
)

if(CONNFWK_IGNORE_OTA_INTERNAL_FLASH)
    list(REMOVE_ITEM SOURCES Source/OtaInternalFlash.c)
endif()

if(CONNFWK_IGNORE_OTA_EXTERNAL_FLASH)
    list(REMOVE_ITEM SOURCES Source/OtaExternalFlash.c)
endif()


add_library(connfwk-OtaSupport ${SOURCES})

target_include_directories(connfwk-OtaSupport
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Interface
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-OtaSupport
    PRIVATE
        connfwk-config
        connfwk-FunctionLib
    PUBLIC
        ${CONNFWK_MCUX_SDK_LIB}
        ${CONNFWK_PLATFORM_LIB}
)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
    add_library(connfwk-OtaSupport-coverage ${SOURCES})
    target_link_libraries(connfwk-OtaSupport-coverage
        PRIVATE 
            connfwk-config
            connfwk-FunctionLib
            ${CONNFWK_PLATFORM_LIB}
            ${CONNFWK_MCUX_SDK_LIB}
        )
    target_include_directories(connfwk-OtaSupport-coverage
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/Interface
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/Source
    )
    
    set(COVERAGE_COMPILE_OPTIONS
        --cs-on
        --cs-hit
        --cs-no-execution-time
    )

    target_compile_options(connfwk-OtaSupport-coverage PRIVATE
            $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
            $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
    )
    set_target_properties(connfwk-OtaSupport-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()
