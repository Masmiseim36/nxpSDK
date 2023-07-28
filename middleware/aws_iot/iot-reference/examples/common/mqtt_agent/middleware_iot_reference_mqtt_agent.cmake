# Add set(CONFIG_USE_middleware_iot_reference_mqtt_agent true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel AND CONFIG_USE_middleware_freertos_backoffalgorithm AND CONFIG_USE_middleware_freertos_coremqtt-agent AND CONFIG_USE_middleware_freertos_corepkcs11 AND CONFIG_USE_middleware_iot_reference_kvstore AND CONFIG_USE_middleware_iot_reference_mqtt_agent_interface AND CONFIG_USE_middleware_iot_reference_template AND ((CONFIG_USE_middleware_iot_reference_transport_mbedtls AND (NOT CONFIG_USE_middleware_iot_reference_transport_mbedtls_wifi_serial)) OR (CONFIG_USE_middleware_iot_reference_transport_mbedtls_wifi_serial AND (NOT CONFIG_USE_middleware_iot_reference_transport_mbedtls))))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/mqtt_agent_task.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "middleware_iot_reference_mqtt_agent dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
