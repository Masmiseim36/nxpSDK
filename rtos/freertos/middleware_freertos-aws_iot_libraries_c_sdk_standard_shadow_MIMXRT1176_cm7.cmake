include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_c_sdk_standard_shadow component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_api.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_operation.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_parser.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_static_memory.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/aws/shadow/src/aws_iot_shadow_subscription.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/aws/shadow/src/aws_shadow.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/aws/shadow/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/aws/shadow/include/types
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/aws/shadow/src
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/aws/shadow/src/private
)


include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_mqtt_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_serializer_MIMXRT1176_cm7)

