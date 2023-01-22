include_guard()
message("middleware_amazon-freertos_libraries_abstractions_mqtt_agent component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/mqtt_agent/freertos_agent_message.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/mqtt_agent/freertos_command_pool.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libraries/abstractions/mqtt_agent/include
)


include(middleware_freertos_coremqtt-agent_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

