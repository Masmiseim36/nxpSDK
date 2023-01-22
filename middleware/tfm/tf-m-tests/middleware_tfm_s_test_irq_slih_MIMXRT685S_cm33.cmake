include_guard()
message("middleware_tfm_s_test_irq_slih component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_slih_test_service/auto_generated/intermedia_tfm_slih_test_service.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_slih_test_service/auto_generated/load_info_tfm_slih_test_service.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/irq/service/tfm_slih_test_service/tfm_slih_test_service.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/service/tfm_irq_test_service.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/irq/service/tfm_slih_test_service
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_slih_test_service
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_slih_test_service/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/spm/common/service
)


