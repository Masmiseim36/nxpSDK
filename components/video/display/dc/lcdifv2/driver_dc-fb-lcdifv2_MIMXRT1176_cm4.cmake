include_guard(GLOBAL)
message("driver_dc-fb-lcdifv2 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_dc_fb_lcdifv2.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_dc-fb-common_MIMXRT1176_cm4)

include(driver_display-common_MIMXRT1176_cm4)

include(driver_lcdifv2_MIMXRT1176_cm4)

