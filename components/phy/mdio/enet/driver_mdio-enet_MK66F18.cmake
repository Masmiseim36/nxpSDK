if(NOT DRIVER_MDIO-ENET_MK66F18_INCLUDED)
    
    set(DRIVER_MDIO-ENET_MK66F18_INCLUDED true CACHE BOOL "driver_mdio-enet component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_enet_mdio.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_mdio-common_MK66F18)

    include(driver_enet_MK66F18)

endif()
