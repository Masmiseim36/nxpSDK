include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_corehttp component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreHTTP/source/core_http_client.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreHTTP/source/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreHTTP/source/interface
)


include(middleware_freertos-aws_iot_libraries_3rdparty_http_parser_MIMXRT1176_cm7)

