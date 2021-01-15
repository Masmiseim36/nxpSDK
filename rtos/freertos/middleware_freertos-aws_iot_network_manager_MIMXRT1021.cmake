if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_NETWORK_MANAGER_MIMXRT1021_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_NETWORK_MANAGER_MIMXRT1021_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_network_manager component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/demos/network_manager/aws_iot_network_manager.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/demos/include
        ${CMAKE_CURRENT_LIST_DIR}/demos/network_manager
    )


    include(middleware_freertos-aws_iot_common_MIMXRT1021)

    include(middleware_freertos-aws_iot_demos_MIMXRT1021)

    include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1021)

    include(middleware_freertos-aws_iot_libraries_abstractions_wifi_MIMXRT1021)

    include(middleware_freertos-kernel_MIMXRT1021)

endif()
