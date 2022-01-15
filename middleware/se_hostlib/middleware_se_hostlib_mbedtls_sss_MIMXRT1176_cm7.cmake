include_guard(GLOBAL)
message("middleware_se_hostlib_mbedtls_sss component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/ecdh_alt_ax.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/ecdsa_verify_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/ecp_alt_sss.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/sss_mbedtls.c
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/sss_mbedtls_rsa.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls
    ${CMAKE_CURRENT_LIST_DIR}/sss/port/ksdk
)


include(middleware_se_hostlib_commonSe050_MIMXRT1176_cm7)

include(middleware_se_hostlib_mbedtls_alt_demo_common_MIMXRT1176_cm7)

