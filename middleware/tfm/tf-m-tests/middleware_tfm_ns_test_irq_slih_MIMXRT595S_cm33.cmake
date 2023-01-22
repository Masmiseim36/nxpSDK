include_guard()
message("middleware_tfm_ns_test_irq_slih component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/suites/irq_test.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/irq/irq_testsuite.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/suites
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/service
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/irq
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common
)


