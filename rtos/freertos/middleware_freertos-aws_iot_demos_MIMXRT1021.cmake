include_guard(GLOBAL)
message("middleware_freertos-aws_iot_demos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/demo_runner/aws_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/demos/demo_runner/iot_demo_freertos.c
    ${CMAKE_CURRENT_LIST_DIR}/demos/network_manager/aws_iot_demo_network.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/include
)


include(middleware_freertos-kernel_MIMXRT1021)

include(middleware_freertos-aws_iot_network_manager_MIMXRT1021)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1021)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT1021)

include(middleware_freertos-aws_iot_mqtt_MIMXRT1021)

