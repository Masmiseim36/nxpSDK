if(NOT MIDDLEWARE_LWIP_APPS_HTTPSSRV_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_LWIP_APPS_HTTPSSRV_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_lwip_apps_httpssrv component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv
    )

    include(middleware_lwip_apps_httpsrv_MIMXRT1062)

    include(middleware_mbedtls_MIMXRT1062)

endif()
