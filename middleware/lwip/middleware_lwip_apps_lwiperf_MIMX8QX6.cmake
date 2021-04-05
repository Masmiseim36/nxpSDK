if(NOT MIDDLEWARE_LWIP_APPS_LWIPERF_MIMX8QX6_INCLUDED)
    
    set(MIDDLEWARE_LWIP_APPS_LWIPERF_MIMX8QX6_INCLUDED true CACHE BOOL "middleware_lwip_apps_lwiperf component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/apps/lwiperf/lwiperf.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/include/lwip/apps
    )


    #OR Logic component
    if(CONFIG_USE_middleware_freertos-kernel_MIMX8QX6)
         include(middleware_freertos-kernel_MIMX8QX6)
    endif()
    if(CONFIG_USE_middleware_baremetal_MIMX8QX6)
         include(middleware_baremetal_MIMX8QX6)
    endif()
    if(NOT (CONFIG_USE_middleware_freertos-kernel_MIMX8QX6 OR CONFIG_USE_middleware_baremetal_MIMX8QX6))
        message(WARNING "Since middleware_freertos-kernel_MIMX8QX6/middleware_baremetal_MIMX8QX6 is not included at first or config in config.cmake file, use middleware_freertos-kernel_MIMX8QX6 by default.")
        include(middleware_freertos-kernel_MIMX8QX6)
    endif()

    include(middleware_lwip_MIMX8QX6)

endif()
