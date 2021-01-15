if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_DEMOS_GREENGRASS_DISCOVERY_MIMXRT1021_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_DEMOS_GREENGRASS_DISCOVERY_MIMXRT1021_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_demos_greengrass_discovery component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/demos/greengrass_connectivity/aws_greengrass_discovery_demo.c
    )


    include(middleware_freertos-aws_iot_demos_MIMXRT1021)

    include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1021)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_aws_greengrass_MIMXRT1021)

    include(middleware_freertos-aws_iot_mqtt_MIMXRT1021)

    include(middleware_freertos-kernel_MIMXRT1021)

endif()
