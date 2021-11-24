if(NOT DRIVER_DISPLAY-MIPI-DSI-CMD_MIMXRT117H_INCLUDED)
    
    set(DRIVER_DISPLAY-MIPI-DSI-CMD_MIMXRT117H_INCLUDED true CACHE BOOL "driver_display-mipi-dsi-cmd component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_mipi_dsi_cmd.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_driver_mipi_dsi_split_MIMXRT117H)
         include(driver_mipi_dsi_split_MIMXRT117H)
    endif()
    if(NOT (CONFIG_USE_driver_mipi_dsi_split_MIMXRT117H))
        message(WARNING "Since driver_mipi_dsi_split_MIMXRT117H is not included at first or config in config.cmake file, use driver_mipi_dsi_MIMXRT117H by default.")
        include(driver_mipi_dsi_MIMXRT117H)
    endif()

    include(driver_common_MIMXRT117H)

endif()
