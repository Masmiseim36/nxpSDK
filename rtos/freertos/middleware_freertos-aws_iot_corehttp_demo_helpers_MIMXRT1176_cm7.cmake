include_guard(GLOBAL)
message("middleware_freertos-aws_iot_corehttp_demo_helpers component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/common/http_demo_helpers/http_demo_utils.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/common/http_demo_helpers
)


include(middleware_freertos-aws_iot_pkcs11_helpers_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_3rdparty_http_parser_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_backoff_algorithm_MIMXRT1176_cm7)

