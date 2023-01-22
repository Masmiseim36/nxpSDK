include_guard()
message("middleware_amazon-freertos_network_manager component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/network_manager/aws_iot_network_manager.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/demos/include
    ${CMAKE_CURRENT_LIST_DIR}/demos/network_manager
)


include(middleware_amazon-freertos_demos_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_platform_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

