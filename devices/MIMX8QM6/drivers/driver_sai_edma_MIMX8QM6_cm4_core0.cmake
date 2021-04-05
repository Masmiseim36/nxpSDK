if(NOT DRIVER_SAI_EDMA_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_SAI_EDMA_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_sai_edma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_sai_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dma3_MIMX8QM6_cm4_core0)

    include(driver_sai_MIMX8QM6_cm4_core0)

endif()
