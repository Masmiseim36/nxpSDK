include_guard()
message("driver_display-mipi-dsi-cmd component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_mipi_dsi_cmd.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_driver_mipi_dsi_MIMXRT595S_cm33)
     include(driver_mipi_dsi_MIMXRT595S_cm33)
endif()
if(NOT (CONFIG_USE_driver_mipi_dsi_MIMXRT595S_cm33))
    message(WARNING "Since driver_mipi_dsi_MIMXRT595S_cm33 is not included at first or config in config.cmake file, use driver_mipi_dsi_MIMXRT595S_cm33 by default.")
    include(driver_mipi_dsi_MIMXRT595S_cm33)
endif()

include(driver_common_MIMXRT595S_cm33)

