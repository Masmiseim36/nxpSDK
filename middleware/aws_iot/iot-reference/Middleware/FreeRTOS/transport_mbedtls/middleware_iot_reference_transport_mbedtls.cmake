# Add set(CONFIG_USE_middleware_iot_reference_transport_mbedtls true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos_coremqtt AND CONFIG_USE_middleware_freertos_corepkcs11 AND CONFIG_USE_middleware_pkcs11 AND CONFIG_USE_middleware_lwip AND CONFIG_USE_middleware_mbedtls AND CONFIG_USE_middleware_iot_reference_template)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/using_mbedtls.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "middleware_iot_reference_transport_mbedtls dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
