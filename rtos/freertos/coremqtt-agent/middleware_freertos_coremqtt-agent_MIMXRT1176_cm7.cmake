include_guard()
message("middleware_freertos_coremqtt-agent component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/core_mqtt_agent_command_functions.c
    ${CMAKE_CURRENT_LIST_DIR}/source/core_mqtt_agent.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/source/include
)


include(middleware_freertos_coremqtt_MIMXRT1176_cm7)

