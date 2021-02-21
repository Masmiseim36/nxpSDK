if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_DEMOS_SHADOW_MIMXRT1176_cm4_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_DEMOS_SHADOW_MIMXRT1176_cm4_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_demos_shadow component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/demos/shadow/aws_iot_demo_shadow.c
    )


    include(middleware_freertos-aws_iot_c_sdk_standard_shadow_MIMXRT1176_cm4)

    include(middleware_freertos-aws_iot_c_sdk_standard_serializer_MIMXRT1176_cm4)

    include(middleware_freertos-aws_iot_demos_MIMXRT1176_cm4)

    include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1176_cm4)

    include(middleware_freertos-aws_iot_mqtt_MIMXRT1176_cm4)

endif()
