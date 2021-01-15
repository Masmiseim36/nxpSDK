if(NOT MIDDLEWARE_LWIP_CONTRIB_UDPECHO_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_LWIP_CONTRIB_UDPECHO_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_lwip_contrib_udpecho component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/udpecho/udpecho.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/contrib/apps/udpecho
    )


    include(middleware_lwip_MIMXRT1064)

endif()
