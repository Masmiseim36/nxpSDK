if(NOT DRIVER_FLEXCAN_EDMA_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_FLEXCAN_EDMA_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_flexcan_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexcan_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dma3_MIMX8QM6_cm4_core0)

    include(driver_flexcan_MIMX8QM6_cm4_core0)

endif()
