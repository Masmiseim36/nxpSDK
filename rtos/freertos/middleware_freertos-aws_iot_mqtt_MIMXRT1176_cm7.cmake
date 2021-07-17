include_guard(GLOBAL)
message("middleware_freertos-aws_iot_mqtt component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_agent.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_api.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_context_connection.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_managed_function_wrapper.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_mutex_wrapper.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_network.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_operation.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_publish_duplicates.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_serializer_deserializer_wrapper.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_static_memory.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_subscription.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_subscription_container.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_validate.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/include/types
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/private
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src
)


include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1176_cm7)

