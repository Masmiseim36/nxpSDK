include_guard(GLOBAL)
message("middleware_mbedtls_port_ksdk component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ksdk_mbedtls.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/des_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/aes_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ecp_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ecp_curves_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ecp_alt_ksdk.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk
)


#OR Logic component
if(CONFIG_USE_middleware_mbedtls_kinetis_MIMXRT1021)
     include(middleware_mbedtls_kinetis_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis1_MIMXRT1021)
     include(middleware_mbedtls_kinetis1_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis2_MIMXRT1021)
     include(middleware_mbedtls_kinetis2_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis3_MIMXRT1021)
     include(middleware_mbedtls_kinetis3_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis4_MIMXRT1021)
     include(middleware_mbedtls_kinetis4_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis5_MIMXRT1021)
     include(middleware_mbedtls_kinetis5_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis6_MIMXRT1021)
     include(middleware_mbedtls_kinetis6_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc1_MIMXRT1021)
     include(middleware_mbedtls_lpc1_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc2_MIMXRT1021)
     include(middleware_mbedtls_lpc2_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc3_MIMXRT1021)
     include(middleware_mbedtls_lpc3_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc4_MIMXRT1021)
     include(middleware_mbedtls_lpc4_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_rt_MIMXRT1021)
     include(middleware_mbedtls_rt_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_rt1_MIMXRT1021)
     include(middleware_mbedtls_rt1_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_rt2_MIMXRT1021)
     include(middleware_mbedtls_rt2_MIMXRT1021)
endif()
if(CONFIG_USE_middleware_mbedtls_empty_MIMXRT1021)
     include(middleware_mbedtls_empty_MIMXRT1021)
endif()
if(NOT (CONFIG_USE_middleware_mbedtls_kinetis_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_kinetis1_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_kinetis2_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_kinetis3_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_kinetis4_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_kinetis5_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_kinetis6_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_lpc1_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_lpc2_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_lpc3_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_lpc4_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_rt_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_rt1_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_rt2_MIMXRT1021 OR CONFIG_USE_middleware_mbedtls_empty_MIMXRT1021))
    message(WARNING "Since middleware_mbedtls_kinetis_MIMXRT1021/middleware_mbedtls_kinetis1_MIMXRT1021/middleware_mbedtls_kinetis2_MIMXRT1021/middleware_mbedtls_kinetis3_MIMXRT1021/middleware_mbedtls_kinetis4_MIMXRT1021/middleware_mbedtls_kinetis5_MIMXRT1021/middleware_mbedtls_kinetis6_MIMXRT1021/middleware_mbedtls_lpc1_MIMXRT1021/middleware_mbedtls_lpc2_MIMXRT1021/middleware_mbedtls_lpc3_MIMXRT1021/middleware_mbedtls_lpc4_MIMXRT1021/middleware_mbedtls_rt_MIMXRT1021/middleware_mbedtls_rt1_MIMXRT1021/middleware_mbedtls_rt2_MIMXRT1021/middleware_mbedtls_empty_MIMXRT1021 is not included at first or config in config.cmake file, use middleware_mbedtls_empty_MIMXRT1021 by default.")
    include(middleware_mbedtls_empty_MIMXRT1021)
endif()

include(middleware_mbedtls_MIMXRT1021)

