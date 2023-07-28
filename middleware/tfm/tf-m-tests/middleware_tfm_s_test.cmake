# Add set(CONFIG_USE_middleware_tfm_s_test true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_s_test_profile_large OR CONFIG_USE_middleware_tfm_s_test_profile_medium OR CONFIG_USE_middleware_tfm_s_test_profile_small)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/secure_suites.c
  ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework.c
  ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework_helpers.c
  ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework_integ_test_helper.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_service/auto_generated/intermedia_tfm_secure_client_service.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_service/auto_generated/load_info_tfm_secure_client_service.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_service/tfm_secure_client_service.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_2/auto_generated/intermedia_tfm_secure_client_2.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_2/auto_generated/load_info_tfm_secure_client_2.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_2/tfm_secure_client_2.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_2/tfm_secure_client_2_api.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/test
  ${CMAKE_CURRENT_LIST_DIR}/test/framework
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_service
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_2
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/secure
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_service/psa_manifest
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_service
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_2/psa_manifest
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_2
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTEST_FRAMEWORK_S
    -DUSE_SP_LOG
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_s_test dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
