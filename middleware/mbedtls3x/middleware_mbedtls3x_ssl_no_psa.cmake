# Add set(CONFIG_USE_middleware_mbedtls3x_ssl_no_psa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedtls3x_crypto_no_psa AND CONFIG_USE_middleware_mbedtls3x_x509)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/library/debug.c
  ${CMAKE_CURRENT_LIST_DIR}/library/mps_reader.c
  ${CMAKE_CURRENT_LIST_DIR}/library/mps_trace.c
  ${CMAKE_CURRENT_LIST_DIR}/library/net_sockets.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_cache.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_ciphersuites.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_client.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_cookie.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_debug_helpers_generated.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_msg.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_ticket.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls12_client.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls12_server.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls13_client.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls13_generic.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls13_keys.c
  ${CMAKE_CURRENT_LIST_DIR}/library/ssl_tls13_server.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/library
)

else()

message(SEND_ERROR "middleware_mbedtls3x_ssl_no_psa dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
