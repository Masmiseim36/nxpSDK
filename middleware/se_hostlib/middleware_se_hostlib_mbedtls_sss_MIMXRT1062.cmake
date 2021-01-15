if(NOT MIDDLEWARE_SE_HOSTLIB_MBEDTLS_SSS_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_SE_HOSTLIB_MBEDTLS_SSS_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_se_hostlib_mbedtls_sss component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/sss_mbedtls.c
        ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/sss_mbedtls_rsa.c
        ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/ecp_alt_sss.c
        ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls/ecdh_alt_ax.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sss/plugin/mbedtls
    )


    include(middleware_se_hostlib_commonSe050_MIMXRT1062)

    include(middleware_se_hostlib_mbedtls_alt_demo_common_MIMXRT1062)

endif()
