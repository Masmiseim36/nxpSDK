# Add set(CONFIG_USE_middleware_mbedtls3x_port_tfm true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedtls3x_port_config AND CONFIG_USE_middleware_mbedtls3x_no_psa)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/tfm/entropy_poll_alt.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_client.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/tfm
)

else()

message(SEND_ERROR "middleware_mbedtls3x_port_tfm dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
