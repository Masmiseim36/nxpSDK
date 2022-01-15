include_guard(GLOBAL)
message("middleware_freertos-aws_iot_demos_wifi_provisioning component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/wifi_provisioning/aws_wifi_connect_task.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/wifi_provisioning
)


include(middleware_freertos-aws_iot_demos_ble_MIMXRT1176_cm7)

