include_guard()
message("middleware_tfm_s_crypto component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/auto_generated/intermedia_tfm_crypto.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/auto_generated/load_info_tfm_crypto.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/psa_driver_api/tfm_builtin_key_loader.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_aead.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_alloc.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_asymmetric.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_cipher.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_hash.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_init.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_key.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_key_derivation.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_key_management.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_mac.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/crypto_rng.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/tfm_crypto_secure_api.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/tfm_mbedcrypto_builtin_keys.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto/psa_driver_api
)


