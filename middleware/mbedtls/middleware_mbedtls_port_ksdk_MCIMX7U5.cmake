if(NOT MIDDLEWARE_MBEDTLS_PORT_KSDK_MCIMX7U5_INCLUDED)
    
    set(MIDDLEWARE_MBEDTLS_PORT_KSDK_MCIMX7U5_INCLUDED true CACHE BOOL "middleware_mbedtls_port_ksdk component is included.")

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
    if(CONFIG_USE_middleware_mbedtls_kinetis_MCIMX7U5)
         include(middleware_mbedtls_kinetis_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis1_MCIMX7U5)
         include(middleware_mbedtls_kinetis1_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis2_MCIMX7U5)
         include(middleware_mbedtls_kinetis2_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis3_MCIMX7U5)
         include(middleware_mbedtls_kinetis3_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis4_MCIMX7U5)
         include(middleware_mbedtls_kinetis4_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis5_MCIMX7U5)
         include(middleware_mbedtls_kinetis5_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis6_MCIMX7U5)
         include(middleware_mbedtls_kinetis6_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_lpc1_MCIMX7U5)
         include(middleware_mbedtls_lpc1_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_lpc2_MCIMX7U5)
         include(middleware_mbedtls_lpc2_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_lpc3_MCIMX7U5)
         include(middleware_mbedtls_lpc3_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_lpc4_MCIMX7U5)
         include(middleware_mbedtls_lpc4_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_rt_MCIMX7U5)
         include(middleware_mbedtls_rt_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_rt1_MCIMX7U5)
         include(middleware_mbedtls_rt1_MCIMX7U5)
    endif()
    if(CONFIG_USE_middleware_mbedtls_rt2_MCIMX7U5)
         include(middleware_mbedtls_rt2_MCIMX7U5)
    endif()
    if(NOT (CONFIG_USE_middleware_mbedtls_kinetis_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_kinetis1_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_kinetis2_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_kinetis3_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_kinetis4_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_kinetis5_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_kinetis6_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_lpc1_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_lpc2_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_lpc3_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_lpc4_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_rt_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_rt1_MCIMX7U5 OR CONFIG_USE_middleware_mbedtls_rt2_MCIMX7U5))
        message(WARNING "Since middleware_mbedtls_kinetis_MCIMX7U5/middleware_mbedtls_kinetis1_MCIMX7U5/middleware_mbedtls_kinetis2_MCIMX7U5/middleware_mbedtls_kinetis3_MCIMX7U5/middleware_mbedtls_kinetis4_MCIMX7U5/middleware_mbedtls_kinetis5_MCIMX7U5/middleware_mbedtls_kinetis6_MCIMX7U5/middleware_mbedtls_lpc1_MCIMX7U5/middleware_mbedtls_lpc2_MCIMX7U5/middleware_mbedtls_lpc3_MCIMX7U5/middleware_mbedtls_lpc4_MCIMX7U5/middleware_mbedtls_rt_MCIMX7U5/middleware_mbedtls_rt1_MCIMX7U5/middleware_mbedtls_rt2_MCIMX7U5 is not included at first or config in config.cmake file, use middleware_mbedtls_kinetis_MCIMX7U5 by default.")
        include(middleware_mbedtls_kinetis_MCIMX7U5)
    endif()

endif()
