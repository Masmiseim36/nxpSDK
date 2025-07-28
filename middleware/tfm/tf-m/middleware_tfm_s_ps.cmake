# Add set(CONFIG_USE_middleware_tfm_s_ps true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_ps_api.c
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/auto_generated/intermedia_tfm_protected_storage.c
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/crypto/ps_crypto_interface.c
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/ps_encrypted_object.c
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/ps_object_system.c
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/ps_object_table.c
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/ps_utils.c
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/tfm_protected_storage.c
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/tfm_ps_req_mngr.c
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/nv_counters/ps_nv_counters.c
          ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_crypto_api.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/include
          ${CMAKE_CURRENT_LIST_DIR}/config
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/core
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include
          ${CMAKE_CURRENT_LIST_DIR}/lib/fih/inc
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/lib/runtime/include
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common
          ${CMAKE_CURRENT_LIST_DIR}/lib/ext/mbedcrypto/mbedcrypto_config
          ${CMAKE_CURRENT_LIST_DIR}/platform/include
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/Device/Config
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/Device/Include
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include/interface
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/driver
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto
          ${CMAKE_CURRENT_LIST_DIR}/interface/src
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/crypto
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage/nv_counters
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DTFM_PARTITION_PROTECTED_STORAGE
                        -DTFM_PARTITION_INTERNAL_TRUSTED_STORAGE
                        -DPLATFORM_DEFAULT_CRYPTO_KEYS
                        -DMBEDTLS_PSA_CRYPTO_DRIVERS
                        -DPS_ENCRYPTION
                        -DPS_ROLLBACK_PROTECTION
                        -DPS_CRYPTO_AEAD_ALG=PSA_ALG_CCM
              )
  
  
  endif()

