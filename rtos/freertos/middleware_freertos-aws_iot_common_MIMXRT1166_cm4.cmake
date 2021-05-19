if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_COMMON_MIMXRT1166_cm4_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_COMMON_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/iot_device_metrics.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/iot_init.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/iot_static_memory_common.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/logging/iot_logging.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/logging/iot_logging_task_dynamic_buffers.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/taskpool/iot_taskpool.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/taskpool/iot_taskpool_static_memory.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/include
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/include/private
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/common/include/types
    )


    include(middleware_freertos-kernel_MIMXRT1166_cm4)

    include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1166_cm4)

endif()
