include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_c_sdk_standard_ble_mt component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/ble/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/ble/src
)

