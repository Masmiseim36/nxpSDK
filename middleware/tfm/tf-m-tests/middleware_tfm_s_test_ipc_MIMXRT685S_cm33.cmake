include_guard()
message("middleware_tfm_s_test_ipc component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/suites/client_api_tests.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/service/client_api_test_service.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/secure/ipc_s_interface_testsuite.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ipc_service/auto_generated/intermedia_tfm_ipc_service_test.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ipc_service/auto_generated/load_info_tfm_ipc_service_test.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/service/tfm_ipc_service/tfm_ipc_service_test.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ipc_client/auto_generated/intermedia_tfm_ipc_client_test.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ipc_client/auto_generated/load_info_tfm_ipc_client_test.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/service/tfm_ipc_client/tfm_ipc_client_test.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/secure
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ipc_service/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ipc_service
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/suites
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/service
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/service/tfm_ipc_service
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ipc_client/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_ipc_client
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/ipc/service/tfm_ipc_client
)


