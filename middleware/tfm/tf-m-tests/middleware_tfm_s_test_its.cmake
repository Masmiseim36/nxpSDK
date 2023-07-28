# Add set(CONFIG_USE_middleware_tfm_s_test_its true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/its/its_tests_common.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/its/secure/psa_its_s_interface_testsuite.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/its/secure/psa_its_s_reliability_testsuite.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/its
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/its/secure
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTEST_S_ITS
  )

endif()

