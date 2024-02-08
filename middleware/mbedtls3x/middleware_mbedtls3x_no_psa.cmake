# Add set(CONFIG_USE_middleware_mbedtls3x_no_psa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedtls3x_template AND CONFIG_USE_middleware_mbedtls3x_crypto_no_psa AND CONFIG_USE_middleware_mbedtls3x_ssl_no_psa AND CONFIG_USE_middleware_mbedtls3x_x509)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/library
)

else()

message(SEND_ERROR "middleware_mbedtls3x_no_psa dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
