# Add set(CONFIG_USE_middleware_freertos_corepkcs11_mbedtls true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_pkcs11 AND CONFIG_USE_middleware_mbedtls AND CONFIG_USE_middleware_freertos_corepkcs11 AND CONFIG_USE_middleware_freertos_corepkcs11_template)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/portable/mbedtls/core_pkcs11_mbedtls.c
)

else()

message(SEND_ERROR "middleware_freertos_corepkcs11_mbedtls dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
