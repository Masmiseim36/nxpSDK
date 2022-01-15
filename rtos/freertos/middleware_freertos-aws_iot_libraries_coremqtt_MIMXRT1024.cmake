include_guard(GLOBAL)
message("middleware_freertos-aws_iot_libraries_coremqtt component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/source/core_mqtt.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/source/core_mqtt_serializer.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/source/core_mqtt_state.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/source/include
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/source/interface
)


