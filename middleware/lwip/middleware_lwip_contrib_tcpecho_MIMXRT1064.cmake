if(NOT MIDDLEWARE_LWIP_CONTRIB_TCPECHO_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_LWIP_CONTRIB_TCPECHO_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_lwip_contrib_tcpecho component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/tcpecho/tcpecho.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/tcpecho
    )


    include(middleware_lwip_MIMXRT1064)

endif()
