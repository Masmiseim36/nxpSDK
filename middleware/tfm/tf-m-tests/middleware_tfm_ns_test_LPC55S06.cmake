include_guard(GLOBAL)
message("middleware_tfm_ns_test component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/app/tfm_integ_test.c
    ${CMAKE_CURRENT_LIST_DIR}/test/framework/non_secure_suites.c
    ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework.c
    ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework_integ_test_helper.c
    ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework_helpers.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/framework
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/core/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/ipc/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/audit/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/t_cose/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/multi_core/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/ps/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/qcbor/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/suites/platform/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(middleware_tfm_memory_utils_LPC55S06)

