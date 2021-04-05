if(NOT MIDDLEWARE_LWIP_ENET_ETHERNETIF_MIMX8QX6_INCLUDED)
    
    set(MIDDLEWARE_LWIP_ENET_ETHERNETIF_MIMX8QX6_INCLUDED true CACHE BOOL "middleware_lwip_enet_ethernetif component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif.c
    )

    if(CONFIG_USE_driver_enet_MIMX8QX6)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif_kinetis.c
    )
    else()
        message(WARNING "please config platform.drivers.enet_MIMX8QX6 or driver.lpc_enet_MIMX8QX6 or driver.enet_qos_MIMX8QX6 first.")
    endif()


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/port
    )


    #OR Logic component
    if(CONFIG_USE_driver_enet_MIMX8QX6)
         include(driver_enet_MIMX8QX6)
    endif()
    if(NOT (CONFIG_USE_driver_enet_MIMX8QX6))
        message(WARNING "Since driver_enet_MIMX8QX6 is not included at first or config in config.cmake file, use driver_enet_MIMX8QX6 by default.")
        include(driver_enet_MIMX8QX6)
    endif()

    include(middleware_lwip_MIMX8QX6)

    include(driver_phy-common_MIMX8QX6)

endif()
