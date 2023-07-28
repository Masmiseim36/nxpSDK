# Add set(CONFIG_USE_middleware_tfm_s_initial_attestation_asymmetric true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_s_initial_attestation)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/attest_asymmetric_key.c
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/tfm_initial_attest_pub_key.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_sign1_sign.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/inc
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DATTEST_KEY_BITS=256
    -DT_COSE_DISABLE_MAC0
    -DT_COSE_DISABLE_ES384
    -DT_COSE_DISABLE_ES512
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_s_initial_attestation_asymmetric dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
