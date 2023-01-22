include_guard()
message("middleware_amazon-freertos_libraries_c_sdk_standard_common component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/iot_device_metrics.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/iot_init.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/iot_static_memory_common.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/taskpool/iot_taskpool.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/taskpool/iot_taskpool_static_memory.c
)


include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_abstractions_platform_MIMXRT595S_cm33)

include(middleware_amazon-freertos_libraries_c_sdk_standard_common_include_MIMXRT595S_cm33)

