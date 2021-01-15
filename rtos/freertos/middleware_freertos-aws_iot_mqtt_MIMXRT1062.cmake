if(NOT MIDDLEWARE_FREERTOS-AWS_IOT_MQTT_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_FREERTOS-AWS_IOT_MQTT_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_freertos-aws_iot_mqtt component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_agent.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_api.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_network.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_operation.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_serialize.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_static_memory.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_subscription.c
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/iot_mqtt_validate.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/include
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/include/types
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src/private
        ${CMAKE_CURRENT_LIST_DIR}/libraries/c_sdk/standard/mqtt/src
    )


    include(middleware_freertos-aws_iot_common_MIMXRT1062)

    include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT1062)

    include(middleware_freertos-kernel_MIMXRT1062)

endif()
