if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_DEMOS_UNUSED_INTERNAL_MT_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_DEMOS_UNUSED_INTERNAL_MT_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_demos_unused_internal_mt component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/demos/ble/aws_ble_gatt_server_demo.c
        ${CMAKE_CURRENT_LIST_DIR}/demos/ble/iot_ble_numericComparison.c
        ${CMAKE_CURRENT_LIST_DIR}/demos/demo_runner/iot_demo_runner.c
        ${CMAKE_CURRENT_LIST_DIR}/demos/mqtt/iot_demo_mqtt.c
        ${CMAKE_CURRENT_LIST_DIR}/demos/wifi_provisioning/aws_wifi_connect_task.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/demos/include
    )


endif()
