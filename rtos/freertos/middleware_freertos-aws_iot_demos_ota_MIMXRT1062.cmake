if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_DEMOS_OTA_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_DEMOS_OTA_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_demos_ota component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/demos/ota/aws_iot_ota_update_demo.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/demos/include
    )


    include(middleware_freertos-aws_iot_common_MIMXRT1062)

    include(middleware_freertos-aws_iot_demos_MIMXRT1062)

    include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1062)

    include(middleware_freertos-aws_iot_libraries_freertos_plus_aws_ota_MIMXRT1062)

    include(middleware_freertos-aws_iot_mqtt_MIMXRT1062)

endif()
