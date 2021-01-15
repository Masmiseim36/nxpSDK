if(NOT DRIVER_FLEXSPI_EDMA_MIMXRT1064_INCLUDED)
    
    set(DRIVER_FLEXSPI_EDMA_MIMXRT1064_INCLUDED true CACHE BOOL "driver_flexspi_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexspi_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexspi_MIMXRT1064)

    include(driver_edma_MIMXRT1064)

endif()
