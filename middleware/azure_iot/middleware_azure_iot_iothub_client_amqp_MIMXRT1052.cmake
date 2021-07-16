include_guard(GLOBAL)
message("middleware_azure_iot_iothub_client_amqp component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransportamqp.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransportamqp_methods.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransport_amqp_cbs_auth.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransport_amqp_common.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransport_amqp_connection.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransport_amqp_device.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransport_amqp_messenger.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransport_amqp_telemetry_messenger.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/iothubtransport_amqp_twin_messenger.c
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/src/uamqp_messaging.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/inc
    ${CMAKE_CURRENT_LIST_DIR}/iothub_client/inc/internal
)


include(middleware_azure_iot_MIMXRT1052)

include(middleware_azure_iot_amqp_MIMXRT1052)

include(middleware_azure_iot_adapters_MIMXRT1052)

include(middleware_azure_iot_adapters_freertos_MIMXRT1052)

include(middleware_azure_iot_iothub_client_MIMXRT1052)

include(middleware_azure_iot_certs_MIMXRT1052)

include(middleware_azure_iot_parson_MIMXRT1052)

include(middleware_azure_iot_umock_c_MIMXRT1052)

include(middleware_azure_iot_azure_macro_utils_MIMXRT1052)

include(middleware_azure_iot_mbedtls_MIMXRT1052)

