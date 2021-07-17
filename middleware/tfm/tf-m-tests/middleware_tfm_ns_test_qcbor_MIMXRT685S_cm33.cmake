include_guard(GLOBAL)
message("middleware_tfm_ns_test_qcbor component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/qcbor/non_secure/qcbor_ns_testsuite.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/qcbor/non_secure
)


