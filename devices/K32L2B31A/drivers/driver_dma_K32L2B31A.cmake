if(NOT DRIVER_DMA_K32L2B31A_INCLUDED)
    
    set(DRIVER_DMA_K32L2B31A_INCLUDED true CACHE BOOL "driver_dma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dmamux_K32L2B31A)

    include(driver_common_K32L2B31A)

endif()
