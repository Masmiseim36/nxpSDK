if(NOT DRIVER_SOC_MIPI_CSI2RX_MIMXRT1166_cm4_INCLUDED)
    
    set(DRIVER_SOC_MIPI_CSI2RX_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "driver_soc_mipi_csi2rx component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_soc_mipi_csi2rx.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1166_cm4)

endif()
