if(NOT MIDDLEWARE_LWIP_APPS_SNTP_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_LWIP_APPS_SNTP_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_lwip_apps_sntp component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/apps/sntp/sntp.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/include/lwip/apps
    )


    include(middleware_lwip_MIMXRT1052)

endif()
