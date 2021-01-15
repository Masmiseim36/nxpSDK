if(NOT DRIVER_LPI2C_EDMA_MIMXRT1052_INCLUDED)
    
    set(DRIVER_LPI2C_EDMA_MIMXRT1052_INCLUDED true CACHE BOOL "driver_lpi2c_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpi2c_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_edma_MIMXRT1052)

    include(driver_lpi2c_MIMXRT1052)

endif()
