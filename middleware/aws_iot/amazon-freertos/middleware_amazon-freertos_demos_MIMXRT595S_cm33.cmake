include_guard()
message("middleware_amazon-freertos_demos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/demo_runner/aws_demo.c
    ${CMAKE_CURRENT_LIST_DIR}/demos/demo_runner/iot_demo_freertos.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/demos/include
)


include(middleware_amazon-freertos_libraries_abstractions_platform_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_c_sdk_standard_common_MIMXRT595S_cm33)

include(middleware_amazon-freertos_network_manager_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

