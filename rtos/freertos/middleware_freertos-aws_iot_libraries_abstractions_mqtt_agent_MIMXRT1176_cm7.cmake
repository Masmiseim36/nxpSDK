include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_abstractions_mqtt_agent component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/mqtt_agent/freertos_agent_message.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/mqtt_agent/freertos_command_pool.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/mqtt_agent/include
)


include(middleware_freertos-aws_iot_libraries_coremqtt-agent_MIMXRT1176_cm7)

include(middleware_freertos-kernel_MIMXRT1176_cm7)

