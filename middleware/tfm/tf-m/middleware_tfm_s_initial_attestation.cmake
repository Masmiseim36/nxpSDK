# Add set(CONFIG_USE_middleware_tfm_s_initial_attestation true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_qcbor)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_attest_api.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/auto_generated/intermedia_tfm_initial_attestation.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/auto_generated/load_info_tfm_initial_attestation.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/attest_boot_data.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/attest_core.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/attest_token_encode.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/tfm_attest.c
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/tfm_attest_req_mngr.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/crypto_adapters/t_cose_psa_crypto.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_parameters.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_util.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/inc
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTFM_PARTITION_INITIAL_ATTESTATION
    -DT_COSE_USE_PSA_CRYPTO
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_s_initial_attestation dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
