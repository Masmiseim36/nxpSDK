include_guard()
message("driver_soc_mipi_dsi component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(driver_common_MIMXRT595S_cm33)

