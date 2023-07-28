# Add set(CONFIG_USE_middleware_tfmtests_ns_test_tcose true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/t_cose/non_secure/t_cose_ns_testsuite.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/t_cose/non_secure
  ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/t_cose
)

