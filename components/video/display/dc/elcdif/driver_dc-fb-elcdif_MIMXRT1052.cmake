include_guard()
message("driver_dc-fb-elcdif component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_dc_fb_elcdif.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_dc-fb-common_MIMXRT1052)

include(driver_elcdif_MIMXRT1052)

