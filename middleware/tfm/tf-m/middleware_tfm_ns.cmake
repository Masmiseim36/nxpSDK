# Add set(CONFIG_USE_middleware_tfm_ns true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/uart_stdout.c
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/libc_dummy.c
          ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_crypto_api.c
          ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_attest_api.c
          ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_its_api.c
          ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_ps_api.c
          ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_platform_api.c
          ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_psa_ns_api.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/platform/include
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/driver
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/Device/Config
          ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/Device/Include
          ${CMAKE_CURRENT_LIST_DIR}/interface/include
          ${CMAKE_CURRENT_LIST_DIR}/interface/include/crypto_keys
          ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa
          ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa_manifest
          ${CMAKE_CURRENT_LIST_DIR}/interface/include/os_wrapper
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/include
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/crypto
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/internal_trusted_storage
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/protected_storage
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/platform
          ${CMAKE_CURRENT_LIST_DIR}/config
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DCONFIG_TFM_FLOAT_ABI=2
                        -DCONFIG_TFM_ENABLE_CP10CP11
                        -DCONFIG_TFM_LAZY_STACKING
                        -D__DOMAIN_NS=1
                        -DDOMAIN_NS=1
                        -DCONFIG_TFM_USE_TRUSTZONE
                        -DATTEST_TOKEN_PROFILE_PSA_IOT_1
                        -DPLATFORM_DEFAULT_CRYPTO_KEYS
                        -DPS_ENCRYPTION
              )
  
  
  endif()

