if(NOT DRIVER_FLEXCOMM_I2C_DMA_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_FLEXCOMM_I2C_DMA_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_flexcomm_i2c_dma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_i2c_dma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexcomm_MIMXRT685S_cm33)

    include(driver_common_MIMXRT685S_cm33)

    include(driver_flexcomm_i2c_MIMXRT685S_cm33)

    include(driver_lpc_dma_MIMXRT685S_cm33)

endif()
