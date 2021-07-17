include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_c_sdk_standard_common component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/iot_device_metrics.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/iot_init.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/iot_static_memory_common.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/taskpool/iot_taskpool.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/taskpool/iot_taskpool_static_memory.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/include/private
    ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/include/types
)


include(middleware_freertos-kernel_MIMXRT1176_cm7)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1176_cm7)

