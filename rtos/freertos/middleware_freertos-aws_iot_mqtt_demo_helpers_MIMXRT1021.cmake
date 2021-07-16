include_guard(GLOBAL)
message("middleware_freertos-aws_iot_mqtt_demo_helpers component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/common/mqtt_demo_helpers/mqtt_demo_helpers.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/common/mqtt_demo_helpers
)


include(middleware_freertos-aws_iot_libraries_abstractions_transport_MIMXRT1021)

include(middleware_freertos-aws_iot_libraries_abstractions_backoff_algorithm_MIMXRT1021)

include(middleware_freertos-aws_iot_pkcs11_helpers_MIMXRT1021)

include(middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1021)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1021)

