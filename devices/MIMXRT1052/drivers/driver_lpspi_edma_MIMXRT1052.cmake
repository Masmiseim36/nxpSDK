if(NOT DRIVER_LPSPI_EDMA_MIMXRT1052_INCLUDED)
    
    set(DRIVER_LPSPI_EDMA_MIMXRT1052_INCLUDED true CACHE BOOL "driver_lpspi_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpspi_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_edma_MIMXRT1052)

    include(driver_lpspi_MIMXRT1052)

endif()
