include_guard(GLOBAL)
message("middleware_tfm_s_test_platform component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/platform/platform_tests_common.c
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/platform/secure/platform_s_interface_testsuite.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/platform
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/platform/secure
)


