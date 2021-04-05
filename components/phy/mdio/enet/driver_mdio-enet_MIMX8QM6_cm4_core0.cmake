if(NOT DRIVER_MDIO-ENET_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_MDIO-ENET_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_mdio-enet component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_enet_mdio.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_mdio-common_MIMX8QM6_cm4_core0)

    include(driver_enet_MIMX8QM6_cm4_core0)

endif()
