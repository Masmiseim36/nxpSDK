include_guard(GLOBAL)
message("middleware_tfm_ns_test_core component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/core/non_secure/core_ns_positive_testsuite.c
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/core/non_secure/core_test_api.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/core/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/test_services/tfm_core_test
)


include(middleware_tfm_platform_test_MIMXRT685S_cm33)

