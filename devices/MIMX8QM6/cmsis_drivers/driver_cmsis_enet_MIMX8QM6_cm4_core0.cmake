if(NOT DRIVER_CMSIS_ENET_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_CMSIS_ENET_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_cmsis_enet component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_enet_cmsis.c
        ${CMAKE_CURRENT_LIST_DIR}/fsl_enet_phy_cmsis.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_enet_MIMX8QM6_cm4_core0)

    include(CMSIS_Driver_Include_Ethernet_MAC_MIMX8QM6_cm4_core0)

    include(CMSIS_Driver_Include_Ethernet_PHY_MIMX8QM6_cm4_core0)

    include(CMSIS_Driver_Include_Ethernet_MIMX8QM6_cm4_core0)

    include(driver_phy-common_MIMX8QM6_cm4_core0)

endif()
