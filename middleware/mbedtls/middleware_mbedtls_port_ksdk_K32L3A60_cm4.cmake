if(NOT MIDDLEWARE_MBEDTLS_PORT_KSDK_K32L3A60_cm4_INCLUDED)
    
    set(MIDDLEWARE_MBEDTLS_PORT_KSDK_K32L3A60_cm4_INCLUDED true CACHE BOOL "middleware_mbedtls_port_ksdk component is included.")

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
    if(CONFIG_USE_middleware_mbedtls_kinetis_K32L3A60_cm4)
         include(middleware_mbedtls_kinetis_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis1_K32L3A60_cm4)
         include(middleware_mbedtls_kinetis1_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis2_K32L3A60_cm4)
         include(middleware_mbedtls_kinetis2_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis3_K32L3A60_cm4)
         include(middleware_mbedtls_kinetis3_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis4_K32L3A60_cm4)
         include(middleware_mbedtls_kinetis4_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis5_K32L3A60_cm4)
         include(middleware_mbedtls_kinetis5_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_kinetis6_K32L3A60_cm4)
         include(middleware_mbedtls_kinetis6_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_lpc1_K32L3A60_cm4)
         include(middleware_mbedtls_lpc1_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_lpc2_K32L3A60_cm4)
         include(middleware_mbedtls_lpc2_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_lpc3_K32L3A60_cm4)
         include(middleware_mbedtls_lpc3_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_lpc4_K32L3A60_cm4)
         include(middleware_mbedtls_lpc4_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_rt_K32L3A60_cm4)
         include(middleware_mbedtls_rt_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_rt1_K32L3A60_cm4)
         include(middleware_mbedtls_rt1_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_middleware_mbedtls_rt2_K32L3A60_cm4)
         include(middleware_mbedtls_rt2_K32L3A60_cm4)
    endif()
    if(NOT (CONFIG_USE_middleware_mbedtls_kinetis_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_kinetis1_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_kinetis2_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_kinetis3_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_kinetis4_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_kinetis5_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_kinetis6_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_lpc1_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_lpc2_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_lpc3_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_lpc4_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_rt_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_rt1_K32L3A60_cm4 OR CONFIG_USE_middleware_mbedtls_rt2_K32L3A60_cm4))
        message(WARNING "Since middleware_mbedtls_kinetis_K32L3A60_cm4/middleware_mbedtls_kinetis1_K32L3A60_cm4/middleware_mbedtls_kinetis2_K32L3A60_cm4/middleware_mbedtls_kinetis3_K32L3A60_cm4/middleware_mbedtls_kinetis4_K32L3A60_cm4/middleware_mbedtls_kinetis5_K32L3A60_cm4/middleware_mbedtls_kinetis6_K32L3A60_cm4/middleware_mbedtls_lpc1_K32L3A60_cm4/middleware_mbedtls_lpc2_K32L3A60_cm4/middleware_mbedtls_lpc3_K32L3A60_cm4/middleware_mbedtls_lpc4_K32L3A60_cm4/middleware_mbedtls_rt_K32L3A60_cm4/middleware_mbedtls_rt1_K32L3A60_cm4/middleware_mbedtls_rt2_K32L3A60_cm4 is not included at first or config in config.cmake file, use middleware_mbedtls_kinetis_K32L3A60_cm4 by default.")
        include(middleware_mbedtls_kinetis_K32L3A60_cm4)
    endif()

endif()
