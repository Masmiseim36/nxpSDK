include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_coremqtt-agent component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/source/core_mqtt_agent.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/source/core_mqtt_agent_command_functions.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/source/include
)


include(middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT1176_cm7)

