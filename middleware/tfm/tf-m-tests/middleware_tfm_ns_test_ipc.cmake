# Add set(CONFIG_USE_middleware_tfm_ns_test_ipc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/suites/client_api_tests.c
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/non_secure/ipc_ns_interface_testsuite.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/suites
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/service
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/non_secure
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DTFM_PARTITION_IPC_TEST
    -DTEST_NS_IPC
  )

endif()

