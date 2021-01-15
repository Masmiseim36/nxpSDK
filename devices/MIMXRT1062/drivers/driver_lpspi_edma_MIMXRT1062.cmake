if(NOT DRIVER_LPSPI_EDMA_MIMXRT1062_INCLUDED)
    
    set(DRIVER_LPSPI_EDMA_MIMXRT1062_INCLUDED true CACHE BOOL "driver_lpspi_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpspi_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_edma_MIMXRT1062)

    include(driver_lpspi_MIMXRT1062)

endif()
