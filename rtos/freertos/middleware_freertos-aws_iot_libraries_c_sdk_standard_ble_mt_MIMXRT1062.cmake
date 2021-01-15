if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_C_SDK_STANDARD_BLE_MT_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_C_SDK_STANDARD_BLE_MT_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_c_sdk_standard_ble_mt component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/ble/src/iot_ble_gap.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/ble/src/iot_ble_gatt.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/ble/src/services/device_information/iot_ble_device_information.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/ble/src/services/mqtt_ble/iot_ble_data_transfer.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/ble/src/services/mqtt_ble/iot_network_ble.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/ble/src/services/wifi_provisioning/iot_ble_wifi_provisioning.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/ble/include
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/ble/src
    )


endif()
