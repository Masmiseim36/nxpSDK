include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_ble component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ble/src/iot_ble_gap.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ble/src/iot_ble_gatt.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ble/src/services/data_transfer/iot_ble_data_transfer.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ble/src/services/device_information/iot_ble_device_information.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ble/src/services/mqtt_ble/iot_ble_mqtt_serialize.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ble/src/services/mqtt_ble/iot_ble_mqtt_transport.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ble/src/services/wifi_provisioning/iot_ble_wifi_provisioning.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ble/src/services/wifi_provisioning/iot_ble_wifi_provisioning_serializer.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ble/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ble/src
)


include(middleware_freertos-aws_iot_libraries_3rdparty_tinycbor_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_ble_hal_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_wifi_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_logging_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

