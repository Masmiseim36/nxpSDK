if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_ABSTRACTIONS_PLATFORM_MIMXRT1166_cm7_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_LIBRARIES_ABSTRACTIONS_PLATFORM_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_libraries_abstractions_platform component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/freertos/iot_clock_freertos.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/freertos/iot_metrics.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/freertos/iot_network_freertos.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/freertos/iot_threads_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/freertos/include/platform
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/include/platform
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/include/types
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/freertos/include
        ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/platform/include
    )


    include(middleware_freertos-aws_iot_common_MIMXRT1166_cm7)

    include(middleware_freertos-aws_iot_libraries_abstractions_secure_sockets_MIMXRT1166_cm7)

    include(middleware_freertos-kernel_MIMXRT1166_cm7)

endif()
