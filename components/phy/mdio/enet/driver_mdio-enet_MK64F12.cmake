if(NOT DRIVER_MDIO-ENET_MK64F12_INCLUDED)
    
    set(DRIVER_MDIO-ENET_MK64F12_INCLUDED true CACHE BOOL "driver_mdio-enet component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_enet_mdio.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_mdio-common_MK64F12)

    include(driver_enet_MK64F12)

endif()
