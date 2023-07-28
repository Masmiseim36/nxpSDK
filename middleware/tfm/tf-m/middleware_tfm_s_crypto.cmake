# Add set(CONFIG_USE_middleware_tfm_s_crypto true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_mbedcrypto_psa_driver_wrapper_default)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_crypto_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/auto_generated/intermedia_tfm_crypto.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/auto_generated/load_info_tfm_crypto.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/psa_driver_api/tfm_builtin_key_loader.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_aead.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_alloc.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_asymmetric.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_cipher.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_hash.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_init.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_key_derivation.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_key_management.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_library.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_mac.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_rng.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/psa_driver_api
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTFM_PARTITION_CRYPTO
    -DPLATFORM_DEFAULT_CRYPTO_KEYS
    -DMBEDTLS_PSA_CRYPTO_DRIVERS
    -DMBEDTLS_PSA_CRYPTO_BUILTIN_KEYS
    -DPSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY
    -DPSA_CRYPTO_DRIVER_TFM_BUILTIN_KEY_LOADER
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_s_crypto dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
