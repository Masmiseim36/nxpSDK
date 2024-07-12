# Add set(CONFIG_USE_middleware_mbedtls3x_crypto true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedtls3x_crypto_no_psa)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_aead.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_cipher.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_client.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_driver_wrappers_no_static.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_ecp.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_ffdh.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_hash.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_mac.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_pake.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_rsa.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_se.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_slot_management.c
  ${CMAKE_CURRENT_LIST_DIR}/library/psa_crypto_storage.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/library
)

else()

message(SEND_ERROR "middleware_mbedtls3x_crypto dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
