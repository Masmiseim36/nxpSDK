if(NOT DRIVER_PHYKSZ8081_MIMXRT1052_INCLUDED)
    
    set(DRIVER_PHYKSZ8081_MIMXRT1052_INCLUDED true CACHE BOOL "driver_phyksz8081 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_phy.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_enet_MIMXRT1052)

endif()
