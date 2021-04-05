if(NOT DRIVER_DISPLAY-MIPI-DSI-CMD_MIMX8QX6_INCLUDED)
    
    set(DRIVER_DISPLAY-MIPI-DSI-CMD_MIMX8QX6_INCLUDED true CACHE BOOL "driver_display-mipi-dsi-cmd component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_mipi_dsi_cmd.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QX6)

    include(driver_mipi_dsi_MIMX8QX6)

endif()
