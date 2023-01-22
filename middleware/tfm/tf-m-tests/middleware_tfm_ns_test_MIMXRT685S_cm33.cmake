include_guard()
message("middleware_tfm_ns_test component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/non_secure_suites.c
    ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework.c
    ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework_integ_test_helper.c
    ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework_helpers.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/framework
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/core/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/audit/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/t_cose/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/multi_core/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/qcbor/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/platform/non_secure
    ${CMAKE_CURRENT_LIST_DIR}/.
)


