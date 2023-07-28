# Add set(CONFIG_USE_middleware_tfm_ns_test true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_tfm_ns_test_profile_large OR CONFIG_USE_middleware_tfm_ns_test_profile_medium OR CONFIG_USE_middleware_tfm_ns_test_profile_small)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/non_secure_suites.c
  ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework.c
  ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework_integ_test_helper.c
  ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework_helpers.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/test/framework
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/non_secure
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/t_cose/non_secure
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/multi_core/non_secure
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/non_secure
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/qcbor/non_secure
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/platform/non_secure
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTEST_FRAMEWORK_NS
  )

endif()

else()

message(SEND_ERROR "middleware_tfm_ns_test dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
