if(NOT DRIVER_LPI2C_EDMA_MCIMX7U5_INCLUDED)
    
    set(DRIVER_LPI2C_EDMA_MCIMX7U5_INCLUDED true CACHE BOOL "driver_lpi2c_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpi2c_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_edma_MCIMX7U5)

    include(driver_lpi2c_MCIMX7U5)

endif()
