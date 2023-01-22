include_guard()
message("middleware_tfm_s_test_ps component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/secure/psa_ps_s_interface_testsuite.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/secure/psa_ps_s_reliability_testsuite.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ps_test_service/auto_generated/intermedia_tfm_ps_test_service.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ps_test_service/auto_generated/load_info_tfm_ps_test_service.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/service/tfm_ps_test_service.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/service/tfm_ps_test_service_api.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/ps/service
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ps_test_service/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ps_test_service
)


