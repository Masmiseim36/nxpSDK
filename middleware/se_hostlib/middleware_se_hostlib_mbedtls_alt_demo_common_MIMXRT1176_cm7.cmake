include_guard(GLOBAL)
message("middleware_se_hostlib_mbedtls_alt_demo_common component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/mbedtls/src/ecdh_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/mbedtls/src/rsa_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/src/mbedtls/fsl_sss_mbedtls_apis.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/hostlib/hostLib/mbedtls/inc
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls
    ${CMAKE_CURRENT_LIST_DIR}/sss/port/ksdk
)


