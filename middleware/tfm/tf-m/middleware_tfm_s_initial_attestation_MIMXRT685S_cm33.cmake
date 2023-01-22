include_guard()
message("middleware_tfm_s_initial_attestation component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/tfm_initial_attest_pub_key.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/auto_generated/intermedia_tfm_initial_attestation.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/auto_generated/load_info_tfm_initial_attestation.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/attest_asymmetric_key.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/attest_boot_data.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/attest_core.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/attest_token_encode.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/tfm_attest.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/tfm_attest_req_mngr.c
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation/tfm_attest_secure_api.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/crypto_adapters/t_cose_psa_crypto.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_parameters.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_sign1_sign.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_util.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/src/ieee754.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/src/qcbor_decode.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/src/qcbor_encode.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/src/UsefulBuf.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/inc
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/src
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/inc
)


