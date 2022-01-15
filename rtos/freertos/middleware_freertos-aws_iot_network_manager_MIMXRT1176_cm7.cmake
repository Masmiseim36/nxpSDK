include_guard(GLOBAL)
message("middleware_freertos-aws_iot_network_manager component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/network_manager/aws_iot_network_manager.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/include
    ${CMAKE_CURRENT_LIST_DIR}/demos/network_manager
)


include(middleware_freertos-aws_iot_demos_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

