include_guard(GLOBAL)
message("middleware_freertos-aws_iot_demos_ble component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/ble/gatt_server/iot_ble_gatt_server_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/demos/ble/numeric_comparison/iot_ble_numericComparison.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/include
)


include(middleware_freertos-aws_iot_libraries_ble_MIMXRT1176_cm7)

