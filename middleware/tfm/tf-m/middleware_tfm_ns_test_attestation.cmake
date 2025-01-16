# Add set(CONFIG_USE_middleware_tfm_ns_test_attestation true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/crypto_adapters/t_cose_psa_crypto.c
          ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_parameters.c
          ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_sign1_sign.c
          ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_util.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/interface/include
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include
          ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include/boot
          ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src
          ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/inc
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DT_COSE_USE_PSA_CRYPTO
              )
  
  
  endif()

