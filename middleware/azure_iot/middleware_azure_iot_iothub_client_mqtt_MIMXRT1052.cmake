include_guard(GLOBAL)
message("middleware_azure_iot_iothub_client_mqtt component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransportmqtt.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransport_mqtt_common.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/inc/internal
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/inc
)


include(middleware_azure_iot_MIMXRT1052)

include(middleware_azure_iot_mqtt_MIMXRT1052)

include(middleware_azure_iot_adapters_MIMXRT1052)

include(middleware_azure_iot_adapters_freertos_MIMXRT1052)

include(middleware_azure_iot_iothub_client_MIMXRT1052)

include(middleware_azure_iot_certs_MIMXRT1052)

include(middleware_azure_iot_parson_MIMXRT1052)

include(middleware_azure_iot_umock_c_MIMXRT1052)

include(middleware_azure_iot_azure_macro_utils_MIMXRT1052)

include(middleware_azure_iot_mbedtls_MIMXRT1052)

