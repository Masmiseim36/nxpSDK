include_guard()
message("middleware_tfm_ns_ipc_api component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_crypto_ipc_api.c
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_initial_attestation_ipc_api.c
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_its_ipc_api.c
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_ps_ipc_api.c
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_platform_ipc_api.c
    ${CMAKE_CURRENT_LIST_DIR}/interface/src/tfm_psa_ns_api.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/interface/include/psa_manifest
    ${CMAKE_CURRENT_LIST_DIR}/interface/include
)


