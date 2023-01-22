include_guard()
message("middleware_tfm_s_test component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/secure_suites.c
    ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework.c
    ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework_helpers.c
    ${CMAKE_CURRENT_LIST_DIR}/test/framework/test_framework_integ_test_helper.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_service/auto_generated/intermedia_tfm_secure_client_service.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_service/auto_generated/load_info_tfm_secure_client_service.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_service/tfm_secure_client_service.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_2/auto_generated/intermedia_tfm_secure_client_2.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_2/auto_generated/load_info_tfm_secure_client_2.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_2/tfm_secure_client_2.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_2/tfm_secure_client_2_api.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
    ${CMAKE_CURRENT_LIST_DIR}/test
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/audit/secure
    ${CMAKE_CURRENT_LIST_DIR}/test/framework
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_service
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/common_test_services/tfm_secure_client_2
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_service/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_service
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_2/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_secure_client_2
)


