include_guard()
message("middleware_psatest_ns_initial_attestation_1 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/targets/common/nspe/initial_attestation/pal_attestation_crypto.c
    ${CMAKE_CURRENT_LIST_DIR}/platform/targets/common/nspe/initial_attestation/pal_attestation_intf.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/initial_attestation/test_a001/test_a001.c
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/initial_attestation/test_a001/test_entry_a001.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/platform/targets/common/nspe/initial_attestation
    ${CMAKE_CURRENT_LIST_DIR}/dev_apis/initial_attestation/test_a001
)


