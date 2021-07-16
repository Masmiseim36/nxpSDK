include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_c_sdk_standard_https component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/https/src/iot_https_client.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/https/src/iot_https_utils.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/https/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/https/include/types
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/https/src/private
)


include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_3rdparty_http_parser_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_corehttp_MIMXRT1062)

