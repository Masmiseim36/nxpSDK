include_guard(GLOBAL)
message("middleware_azure_iot_mqtt component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/umqtt/src/mqtt_client.c
    ${CMAKE_CURRENT_LIST_DIR}/umqtt/src/mqtt_codec.c
    ${CMAKE_CURRENT_LIST_DIR}/umqtt/src/mqtt_message.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/umqtt/inc
)


include(middleware_azure_iot_MIMXRT1052)

