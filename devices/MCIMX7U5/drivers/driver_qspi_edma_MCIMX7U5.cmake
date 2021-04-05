if(NOT DRIVER_QSPI_EDMA_MCIMX7U5_INCLUDED)
    
    set(DRIVER_QSPI_EDMA_MCIMX7U5_INCLUDED true CACHE BOOL "driver_qspi_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_qspi_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_qspi_MCIMX7U5)

    include(driver_edma_MCIMX7U5)

endif()
