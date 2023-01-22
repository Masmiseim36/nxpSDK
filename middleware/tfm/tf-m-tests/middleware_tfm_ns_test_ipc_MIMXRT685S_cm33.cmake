include_guard()
message("middleware_tfm_ns_test_ipc component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/suites/client_api_tests.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/non_secure/ipc_ns_interface_testsuite.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/suites
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/service
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/non_secure
)


