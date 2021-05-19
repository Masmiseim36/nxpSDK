if(NOT DRIVER_PHY-DEVICE-RTL8211F_MIMXRT1166_cm4_INCLUDED)
    
    set(DRIVER_PHY-DEVICE-RTL8211F_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "driver_phy-device-rtl8211f component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_phyrtl8211f.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_phy-common_MIMXRT1166_cm4)

endif()
