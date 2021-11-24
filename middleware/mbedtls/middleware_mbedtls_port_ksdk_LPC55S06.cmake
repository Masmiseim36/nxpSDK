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
if(CONFIG_USE_middleware_mbedtls_kinetis_LPC55S06)
     include(middleware_mbedtls_kinetis_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis1_LPC55S06)
     include(middleware_mbedtls_kinetis1_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis2_LPC55S06)
     include(middleware_mbedtls_kinetis2_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis3_LPC55S06)
     include(middleware_mbedtls_kinetis3_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis4_LPC55S06)
     include(middleware_mbedtls_kinetis4_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis5_LPC55S06)
     include(middleware_mbedtls_kinetis5_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis6_LPC55S06)
     include(middleware_mbedtls_kinetis6_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc1_LPC55S06)
     include(middleware_mbedtls_lpc1_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc2_LPC55S06)
     include(middleware_mbedtls_lpc2_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc3_LPC55S06)
     include(middleware_mbedtls_lpc3_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc4_LPC55S06)
     include(middleware_mbedtls_lpc4_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_rt_LPC55S06)
     include(middleware_mbedtls_rt_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_rt1_LPC55S06)
     include(middleware_mbedtls_rt1_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_rt2_LPC55S06)
     include(middleware_mbedtls_rt2_LPC55S06)
endif()
if(CONFIG_USE_middleware_mbedtls_empty_LPC55S06)
     include(middleware_mbedtls_empty_LPC55S06)
endif()
if(NOT (CONFIG_USE_middleware_mbedtls_kinetis_LPC55S06 OR CONFIG_USE_middleware_mbedtls_kinetis1_LPC55S06 OR CONFIG_USE_middleware_mbedtls_kinetis2_LPC55S06 OR CONFIG_USE_middleware_mbedtls_kinetis3_LPC55S06 OR CONFIG_USE_middleware_mbedtls_kinetis4_LPC55S06 OR CONFIG_USE_middleware_mbedtls_kinetis5_LPC55S06 OR CONFIG_USE_middleware_mbedtls_kinetis6_LPC55S06 OR CONFIG_USE_middleware_mbedtls_lpc1_LPC55S06 OR CONFIG_USE_middleware_mbedtls_lpc2_LPC55S06 OR CONFIG_USE_middleware_mbedtls_lpc3_LPC55S06 OR CONFIG_USE_middleware_mbedtls_lpc4_LPC55S06 OR CONFIG_USE_middleware_mbedtls_rt_LPC55S06 OR CONFIG_USE_middleware_mbedtls_rt1_LPC55S06 OR CONFIG_USE_middleware_mbedtls_rt2_LPC55S06 OR CONFIG_USE_middleware_mbedtls_empty_LPC55S06))
    message(WARNING "Since middleware_mbedtls_kinetis_LPC55S06/middleware_mbedtls_kinetis1_LPC55S06/middleware_mbedtls_kinetis2_LPC55S06/middleware_mbedtls_kinetis3_LPC55S06/middleware_mbedtls_kinetis4_LPC55S06/middleware_mbedtls_kinetis5_LPC55S06/middleware_mbedtls_kinetis6_LPC55S06/middleware_mbedtls_lpc1_LPC55S06/middleware_mbedtls_lpc2_LPC55S06/middleware_mbedtls_lpc3_LPC55S06/middleware_mbedtls_lpc4_LPC55S06/middleware_mbedtls_rt_LPC55S06/middleware_mbedtls_rt1_LPC55S06/middleware_mbedtls_rt2_LPC55S06/middleware_mbedtls_empty_LPC55S06 is not included at first or config in config.cmake file, use middleware_mbedtls_empty_LPC55S06 by default.")
    include(middleware_mbedtls_empty_LPC55S06)
endif()

include(middleware_mbedtls_LPC55S06)

