include_guard()
message("middleware_tfm_ns_test_attestation component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/common/template/tfm_initial_attest_pub_key.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/src/ieee754.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/src/qcbor_decode.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/src/qcbor_encode.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/src/UsefulBuf.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/test/half_to_double_from_rfc7049.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/test/qcbor_encode_tests.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/test/qcbor_decode_tests.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/test/float_tests.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/test/run_tests.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/test/UsefulBuf_Tests.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/crypto_adapters/t_cose_psa_crypto.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_parameters.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_sign1_sign.c
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src/t_cose_util.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/interface/include
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/partitions/initial_attestation
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include
    ${CMAKE_CURRENT_LIST_DIR}/secure_fw/spm/include/boot
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/inc
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/src
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/qcbor/test
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/src
    ${CMAKE_CURRENT_LIST_DIR}/lib/ext/t_cose/inc
    ${CMAKE_CURRENT_LIST_DIR}/bl2/include
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(middleware_tfm_t_cose_sign1_verify_MIMXRT595S_cm33)

include(middleware_tfmtests_ns_test_attestation_MIMXRT595S_cm33)

