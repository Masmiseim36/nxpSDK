include_guard(GLOBAL)
message("middleware_tfm_ns_test_its component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/its/its_tests_common.c
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/its/non_secure/psa_its_ns_interface_testsuite.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/its
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/its/non_secure
)


