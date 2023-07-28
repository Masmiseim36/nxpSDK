# Add set(CONFIG_USE_middleware_tfm_ns_test_attestation_asymmetric true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_ns_test_attestation AND CONFIG_USE_middleware_tfmtests_ns_test_attestation_asymmetric AND CONFIG_USE_middleware_tfm_t_cose_sign1_verify)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/tfm_initial_attest_pub_key.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_sign1_sign.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/inc
)

else()

message(SEND_ERROR "middleware_tfm_ns_test_attestation_asymmetric dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
