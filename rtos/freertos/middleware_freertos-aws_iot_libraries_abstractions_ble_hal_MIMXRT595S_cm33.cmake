include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_abstractions_ble_hal component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/ble_hal/include
)

