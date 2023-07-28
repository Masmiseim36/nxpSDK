# Add set(CONFIG_USE_middleware_mbedcrypto_psa_driver_wrapper_default true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedcrypto)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_driver_wrappers.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include/psa
  ${CMAKE_CURRENT_LIST_DIR}/library
)

else()

message(SEND_ERROR "middleware_mbedcrypto_psa_driver_wrapper_default dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
