include_guard()
message("middleware_tfm_s_test_core component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_core_test/auto_generated/intermedia_tfm_ss_core_test.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_core_test/auto_generated/load_info_tfm_ss_core_test.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/core/service/tfm_core_test/tfm_ss_core_test.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_core_test_2/auto_generated/intermedia_tfm_ss_core_test_2.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_core_test_2/auto_generated/load_info_tfm_ss_core_test_2.c
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/core/service/tfm_core_test_2/tfm_ss_core_test_2.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/core/service/tfm_core_test
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/suites/core/service/tfm_core_test_2
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_core_test/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_core_test
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_core_test_2/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/test/secure_fw/test_services/tfm_core_test_2
)


include(middleware_tfm_platform_test_MIMXRT595S_cm33)

