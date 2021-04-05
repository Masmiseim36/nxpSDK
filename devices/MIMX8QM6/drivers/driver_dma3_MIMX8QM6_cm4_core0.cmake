if(NOT DRIVER_DMA3_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_DMA3_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_dma3 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_edma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_memory_MIMX8QM6_cm4_core0)

endif()
