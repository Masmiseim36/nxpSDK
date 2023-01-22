include_guard()
message("middleware_amazon-freertos_libraries_logging component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/logging/iot_logging.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/logging/iot_logging_task_dynamic_buffers.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/logging/include
)


include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_c_sdk_standard_common_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_platform_MIMXRT595S_cm33)

