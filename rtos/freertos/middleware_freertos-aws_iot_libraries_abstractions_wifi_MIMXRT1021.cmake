include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_abstractions_wifi component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/wifi/include
)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1021)

include(middleware_freertos-kernel_MIMXRT1021)

