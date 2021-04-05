if(NOT DRIVER_MDIO-ENET_MIMX8QX6_INCLUDED)
    
    set(DRIVER_MDIO-ENET_MIMX8QX6_INCLUDED true CACHE BOOL "driver_mdio-enet component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_enet_mdio.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_mdio-common_MIMX8QX6)

    include(driver_enet_MIMX8QX6)

endif()
