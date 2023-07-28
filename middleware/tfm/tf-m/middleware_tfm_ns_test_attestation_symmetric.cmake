# Add set(CONFIG_USE_middleware_tfm_ns_test_attestation_symmetric true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_ns_test_attestation AND CONFIG_USE_middleware_tfmtests_ns_test_attestation_symmetric AND CONFIG_USE_middleware_tfm_t_cose_mac0_verify)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_mac0_sign.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/inc
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DSYMMETRIC_INITIAL_ATTESTATION
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_ns_test_attestation_symmetric dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
