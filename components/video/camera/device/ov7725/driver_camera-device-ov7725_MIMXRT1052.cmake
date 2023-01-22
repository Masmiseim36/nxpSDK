include_guard()
message("driver_camera-device-ov7725 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_ov7725.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_video-common_MIMXRT1052)

include(driver_camera-common_MIMXRT1052)

include(driver_camera-device-common_MIMXRT1052)

include(driver_camera-device-sccb_MIMXRT1052)

