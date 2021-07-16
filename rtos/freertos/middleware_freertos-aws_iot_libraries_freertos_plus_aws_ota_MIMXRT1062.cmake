include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_freertos_plus_aws_ota component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/ota/src/aws_iot_ota_agent.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/ota/src/aws_iot_ota_interface.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/ota/src/http/aws_iot_ota_http.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/ota/src/mqtt/aws_iot_ota_cbor.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/ota/src/mqtt/aws_iot_ota_mqtt.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/ota/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/ota/src
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/ota/src/http
    ${CMAKE_CURRENT_LIST_DIR}/libraries/freertos_plus/aws/ota/src/mqtt
)


include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1062)

include(middleware_freertos-aws_iot_demos_ota_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_3rdparty_jsmn_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_3rdparty_tinycbor_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1062)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_https_MIMXRT1062)

include(middleware_freertos-aws_iot_mqtt_MIMXRT1062)

include(middleware_freertos-kernel_MIMXRT1062)

include(middleware_mbedtls_MIMXRT1062)

