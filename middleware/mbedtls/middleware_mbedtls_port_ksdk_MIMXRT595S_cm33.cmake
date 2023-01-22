include_guard()
message("middleware_mbedtls_port_ksdk component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ksdk_mbedtls.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/des_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/aes_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ecp_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ecp_curves_alt.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk/ecp_alt_ksdk.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/port/ksdk
)


#OR Logic component
if(CONFIG_USE_middleware_mbedtls_common_MIMXRT595S_cm33)
     include(middleware_mbedtls_common_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis_MIMXRT595S_cm33)
     include(middleware_mbedtls_kinetis_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis1_MIMXRT595S_cm33)
     include(middleware_mbedtls_kinetis1_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis2_MIMXRT595S_cm33)
     include(middleware_mbedtls_kinetis2_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis3_MIMXRT595S_cm33)
     include(middleware_mbedtls_kinetis3_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis4_MIMXRT595S_cm33)
     include(middleware_mbedtls_kinetis4_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis5_MIMXRT595S_cm33)
     include(middleware_mbedtls_kinetis5_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_kinetis6_MIMXRT595S_cm33)
     include(middleware_mbedtls_kinetis6_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc1_MIMXRT595S_cm33)
     include(middleware_mbedtls_lpc1_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc2_MIMXRT595S_cm33)
     include(middleware_mbedtls_lpc2_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc3_MIMXRT595S_cm33)
     include(middleware_mbedtls_lpc3_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_lpc4_MIMXRT595S_cm33)
     include(middleware_mbedtls_lpc4_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_rt_MIMXRT595S_cm33)
     include(middleware_mbedtls_rt_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_rt1_MIMXRT595S_cm33)
     include(middleware_mbedtls_rt1_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_rt2_MIMXRT595S_cm33)
     include(middleware_mbedtls_rt2_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_middleware_mbedtls_empty_MIMXRT595S_cm33)
     include(middleware_mbedtls_empty_MIMXRT595S_cm33)
endif()
if(NOT (CONFIG_USE_middleware_mbedtls_common_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_kinetis_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_kinetis1_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_kinetis2_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_kinetis3_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_kinetis4_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_kinetis5_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_kinetis6_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_lpc1_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_lpc2_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_lpc3_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_lpc4_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_rt_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_rt1_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_rt2_MIMXRT595S_cm33 OR CONFIG_USE_middleware_mbedtls_empty_MIMXRT595S_cm33))
    message(WARNING "Since middleware_mbedtls_common_MIMXRT595S_cm33/middleware_mbedtls_kinetis_MIMXRT595S_cm33/middleware_mbedtls_kinetis1_MIMXRT595S_cm33/middleware_mbedtls_kinetis2_MIMXRT595S_cm33/middleware_mbedtls_kinetis3_MIMXRT595S_cm33/middleware_mbedtls_kinetis4_MIMXRT595S_cm33/middleware_mbedtls_kinetis5_MIMXRT595S_cm33/middleware_mbedtls_kinetis6_MIMXRT595S_cm33/middleware_mbedtls_lpc1_MIMXRT595S_cm33/middleware_mbedtls_lpc2_MIMXRT595S_cm33/middleware_mbedtls_lpc3_MIMXRT595S_cm33/middleware_mbedtls_lpc4_MIMXRT595S_cm33/middleware_mbedtls_rt_MIMXRT595S_cm33/middleware_mbedtls_rt1_MIMXRT595S_cm33/middleware_mbedtls_rt2_MIMXRT595S_cm33/middleware_mbedtls_empty_MIMXRT595S_cm33 is not included at first or config in config.cmake file, use middleware_mbedtls_empty_MIMXRT595S_cm33 by default.")
    include(middleware_mbedtls_empty_MIMXRT595S_cm33)
endif()

include(middleware_mbedtls_MIMXRT595S_cm33)

