include_guard()
message("driver_display-rm67162 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_rm67162.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_display-common_MIMXRT595S_cm33)

include(driver_display-mipi-dsi-cmd_MIMXRT595S_cm33)

