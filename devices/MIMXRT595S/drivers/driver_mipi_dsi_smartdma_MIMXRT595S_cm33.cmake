if(NOT DRIVER_MIPI_DSI_SMARTDMA_MIMXRT595S_cm33_INCLUDED)
    
    set(DRIVER_MIPI_DSI_SMARTDMA_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "driver_mipi_dsi_smartdma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_mipi_dsi_smartdma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_mipi_dsi_MIMXRT595S_cm33)

    include(driver_lpc_smartdma_MIMXRT595S_cm33)

endif()
