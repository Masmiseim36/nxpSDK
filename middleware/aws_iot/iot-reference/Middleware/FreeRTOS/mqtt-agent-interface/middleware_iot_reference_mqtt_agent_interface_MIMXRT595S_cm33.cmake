include_guard()
message("middleware_iot_reference_mqtt_agent_interface component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/freertos_agent_message.c
    ${CMAKE_CURRENT_LIST_DIR}/freertos_command_pool.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)


include(middleware_freertos_coremqtt-agent_MIMXRT595S_cm33)

