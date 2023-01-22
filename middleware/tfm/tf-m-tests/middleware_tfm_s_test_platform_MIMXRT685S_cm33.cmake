include_guard()
message("middleware_tfm_s_test_platform component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/platform/platform_tests_common.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/platform/secure/platform_s_interface_testsuite.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/platform
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/platform/secure
)


