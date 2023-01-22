include_guard()
message("driver_camera-receiver-csi component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_csi_camera_adapter.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_video-common_MIMXRT1176_cm7)

include(driver_camera-common_MIMXRT1176_cm7)

include(driver_camera-receiver-common_MIMXRT1176_cm7)

include(driver_csi_MIMXRT1176_cm7)

