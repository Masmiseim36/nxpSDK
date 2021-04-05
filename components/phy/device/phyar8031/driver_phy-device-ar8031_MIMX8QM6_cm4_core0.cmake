if(NOT DRIVER_PHY-DEVICE-AR8031_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_PHY-DEVICE-AR8031_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_phy-device-ar8031 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_phyar8031.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_phy-common_MIMX8QM6_cm4_core0)

endif()
